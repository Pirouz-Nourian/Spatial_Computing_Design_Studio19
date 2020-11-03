//SHAFT PLACEMENT

//Date: 24-01-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Adapted from work by: Shervin Azadi, Dr.ir. Pirouz Nourian, Hans Hoogenboom
//Purpose: To find points within the envelope that, based on sliders and position, are ideal for the placement of lift/stairwell shafts




//SLIDER 'MAX. DIST. SHAFT-FUNCTION'    -> MAXIMUM ALLOWABLE DISTANCE OF SHAFT FROM FUNCTIONS
float max_dist_s_f = chf("max_dist_s_f");

//SLIDER 'MIN. DIST. SHAFT-SHAFT'       -> MINIMUM ALLOWABLE DISTANCE OF SHAFT FROM OTHER SHAFT
float min_dist_s_s = chf("min_dist_s_s");

//SLIDER 'SHAFT COUNT'                  -> MAXIMUM AMOUNT OF SHAFTS TO CREATE
int shaft_count_slider = chi("shaft_count");


//GET X-VALUE OF ALL INITIAL SEEDS (STORE IN LIST -> SEED_POINTS_X)
//GET Z-VALUE OF ALL INITIAL SEEDS (STORE IN LIST -> SEED_POINTS_Z)
float seed_points_x[];
float seed_points_z[];

//PREPARE LIST OF SEED POSITIONS
vector seed_pos[];

//CREATE ITERATOR LIMITERS
int num_points_0 = npoints(0);
int num_points_1 = npoints(1);

//iterating through all points/voxels
//getting position, x, and z values of all seeds
//adding them to respective list
for (int point_id = 0; point_id < num_points_1; point_id++)
{
    int seed = point(1,"parent",point_id); //seeds are presented in parent attribute
    if (seed != -1) // all seeds have value other than -1
    {
        vector pos = point(1,"P",point_id);
        float x_value = pos[0];
        float z_value = pos[2];
        append(seed_points_x,x_value);
        append(seed_points_z,z_value);
        append(seed_pos, pos);
    }
}

//SET SHAFT_COUNT = 0 (A VARIABLE TO BE CHECKED WHEN IT GETS 'FULL' -> ACCORDING TO THE SLIDER 'SHAFT COUNT')
int shaft_count = 0;

//SET SHAFT_ORIGIN_LIST (AN EMPTY LIST OFF ALL SHAFTS CONTAINING THE POSITION VALUE OF ITS ORIGIN-> TO BE FILLED IF THE BELOW CODE SUCCEEDS IN PLACING A SHAFT)
int shaft_origins[];

//GET LOWEST POINTS OF THE ENVELOPE (SHAFT WILL ORIGINATE ON GROUND FLOOR)
int ground_floor[];
for (int point_id = 0; point_id < num_points_0; point_id++)
{
    vector pos = point(0,"P",point_id);
    
    float y_value = pos[1];

    if (abs(y_value) < 0.001) // the lowest y-value equals 0.0
    {
        for (int seed_id = 0; seed_id < len(seed_pos); seed_id++)
        {
            vector this_seed_pos = seed_pos[seed_id];
            this_seed_pos *= set(1,0,1);
            float dist = distance(this_seed_pos, pos);
            if (dist < max_dist_s_f && dist > 5)
            {
                append(ground_floor,point_id);
            }
        }
    }
}


//FOR EVERY LOWEST POINT OF THE ENVELOPE (SHAFT WILL ORIGINATE ON GROUND FLOOR)
for (int ground_point = 0; ground_point < len(ground_floor); ground_point++) // for ground point on ground floor
{
    //IF THE DESIRED SHAFT COUNT HAS BEEN EXCEEDED, BREAK THE LOOP (FINISH SHAFT PLACEMENT)
    if (shaft_count >= shaft_count_slider) break;
    
    //set a toggle for getting out of coming for loops if other if statements are false
    int toggle = 0;
    
    //get the x and z positions of the possible shaft origin point
    vector pos = point(0,"P",ground_floor[ground_point]);
    float x_value = pos[0];
    float z_value = pos[2];
    float shaft_points_x = x_value;
    float shaft_points_z = z_value;

    //IF 'SHAFT_COUNT' IS EQUAL TO 0 (FOR PLACING FIRST SHAFT)
    if (shaft_count == 0)
    {
        //APPEND ITS ORIGIN POINT INTO THE LIST 'SHAFT_ORIGIN_LIST'
        append(shaft_origins,ground_floor[ground_point]);
        //PLACE THE SHAFT AT THAT ORIGIN POINT BY SETTING AN ATTRIBUTE
        setpointattrib(0,"shaft_origins",ground_floor[ground_point],1, "set"); //1 means center of shaft
        setpointgroup(0, "shaft_base", ground_floor[ground_point], 1, "set"); 
        //INCREASE 'SHAFT_COUNT' BY 1
        shaft_count++;
        //GO TO NEXT LOWEST POINT OF THE ENVELOPE (PLACING A SHAFT HERE WILL PUT IT TOO FAR FROM ANY SEED, ACCORDING TO THE SLIDER          
        toggle = 1; // set toggle to 1
    }
    
    //check toggle
    if (toggle) // toggle (1) will equal true
    {
        continue; // continue the loop: go to next iteration of the loop this is nested in
    }
    
    //IF 'SHAFT_COUNT' IS GREATER THAN 0 (CHECK FOR SURROUNDING SHAFTS)
    if (shaft_count > 0)
    {
        //FOR SHAFTS IN 'SHAFT_ORIGIN_LIST'
        for (int i = 0; i < len(shaft_origins); i++)
        {
            //getting x and z value of point
            vector shaft_origin_pos = point(0,"P",shaft_origins[i]);
            float shaft_origin_x = shaft_origin_pos[0];
            float shaft_origin_z = shaft_origin_pos[2];
            
            //calculate distance between this point and other shaft points
            float dist = distance(pos, shaft_origin_pos);
            
            //IF THE DISTANCE BETWEEN THAT ORIGIN POINT AND THE POINT OF ANY EXISTING SHAFT IS LESS THAN 'MIN. DIST. SHAFT-SHAFT'
            if ( dist <= min_dist_s_s )
            {
                //GO TO NEXT LOWEST POINT OF THE ENVELOPE (PLACING A SHAFT HERE WILL PUT IT TOO CLOSE TO ANOTHER SHAFT, ACCORDING TO THE SLIDER)
                toggle = 1; // set toggle to 1
                break; // break the loop: skip all next iterations (of the loop this is nested in)
            }
        }
        
        //check toggle
        if (toggle) // toggle (1) will equal true
        {
            continue; // continue the loop: go to next iterations (of the loop this is nested in)
        }
        
        //APPEND ITS ORIGIN POINT INTO THE LIST 'SHAFT_ORIGIN_LIST'
        append(shaft_origins, ground_floor[ground_point]);
        //PLACE THE SHAFT AT THAT ORIGIN POINT BY SETTING AN ATTRIBUTE
        setpointattrib(0,"shaft_origins",ground_floor[ground_point],1,"set"); //1 means center of shaft
        setpointgroup(0, "shaft_base", ground_floor[ground_point], 1, "set"); 
        //INCREASE 'SHAFT_COUNT' BY 1
        shaft_count++;
    }
}

//FOR EACH POINT ON THE GROUND ADD TO A GROUP ATTRIBUTE
foreach (int ground_point; ground_floor) setpointgroup(0, "ground", ground_point, 1, "set");      

//FOR EACH POINT IN THE ENVELOPE
for (int i = 0; i < num_points_0; i++)
{
    //get a position
    vector pos_2 = point(0,"P",i);
    //project to ground plane
    pos_2 *= set(1,0,1);
    
    //for each shaft
    foreach (int shaft; shaft_origins)
    {
        //get a position
        vector shaft_pos = point(0, "P", shaft);
        //if the shaft position is at the same spot as the projected envelope position
        if ( distance(pos_2,shaft_pos) < 0.001 )
        {
            //set those envelope positions to occupied
            setpointgroup(0,"occupied",i,1,"set");
            break;
        }
    }
}                


