//SEED PLACEMENT

//Date: 24-01-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Adapted from work by: Shervin Azadi, Dr.ir. Pirouz Nourian, Hans Hoogenboom
//Purpose: Determines the best voxel location for each function starting with three most important functions determined by the designer

//A: SEED PLACEMENT OF 3 MOST IMPORTANT FUNCTIONS (CLUSTER START)

//number of functions
//number of points
int num_func = npoints(1);
int num_points = npoints(0);

//initialize lists
int func_seed_pos[];
int occupied_voxels[];


//loop through first 3 to be placed (most important) functions
for (int curr_func = 0; curr_func < 3; curr_func++)
{
    //name of weighted function
    string wpName = "wp" + itoa(curr_func);
    
    float max_weights[];
    int max_id[];
    
    //going through each voxel
    for (int curr_point = 0; curr_point < num_points; curr_point++)
    {
        //weight of voxel, add to list
        float curr_weight = point(0,wpName,curr_point);
        append(max_weights,curr_weight);
        
        //add point id to list
        append(max_id,curr_point);
    }
    
    //sorting list of weights
    int sorted_weights_indicies[] = reverse(argsort(max_weights));
    int sorted_seed_points[] = reorder(max_id,sorted_weights_indicies);

    //initializing index and truth value
    int index = 0;
    int no_result = 0;
    
    //creating while loop
    while ( (no_result == 0) && (index < len(sorted_seed_points)) )
    {
        //getting the point id
        int point_id = sorted_seed_points[index];
        
        //if point is occupied
        if ( (find(occupied_voxels,point_id)) > -1)
        {
            //increasing index, going to next largest point
            index += 1; 
        }
        
        //if point is not occupied
        else
        {
            //adding to list of seed positions
            append(func_seed_pos,point_id);
            
            //add to list of occupied voxels
            append(occupied_voxels,point_id);
            
            //endng while loop, setting no result to 1
            no_result = 1;
        }
    }    
}

//loop through first 3 to be placed (most important) functions
for (int curr_func = 0; curr_func < 3; curr_func++)
{
    int parent = func_seed_pos[curr_func];
    setpointattrib(0, "parent",parent,curr_func, "set");
    setpointgroup(0,"occupied",parent,1,"set");
}



//B: PER REMAINING FUNCTION, DETERMINE DESIRED RELATIONSHIP RADIUS TO INITIALLY SEEDED FUNCTION AND INCREASE WP OF RADIUS POINTS FOR FUNCTION ACCORDINGLY

//number of functions
//number of points
int num_points = npoints(0);
int num_functions = npoints(1);

//user inputs
float rel_distance = chf("relationship_closeness");
float range = chf("range");
int wp_scale = chi("wp_scale_factor");

//creating a list of the attributes to go over
string parameterList[] = array("LivingUnits","StartUpOffices","Arcade","Library","CoworkingSpaces","FabLabsWorkshops","CommunityCentre","EntranceHall","IndoorMarket","Gym","Restaurant","CafePub","Cinema");                

//go through each function you want to seed
for(int func=0; func < num_functions; func++)
{
    //making sure its not the most important function
    if (func > 2)
    {
        //going through each voxel
        for (int voxel = 0; voxel < num_points; voxel++)
        {
            //if the voxel is a seed
            int parent_value = point(0, "parent", voxel);
            if (parent_value > -1)
            {
                //record the position of the voxel
                vector pos_seed = point(0, "P", voxel);
                
                //getting function name of the seed from function ID
                string func_Name = parameterList[parent_value];
                
                //getting relationship between seed and new function
                int relation = point(2, func_Name, func);
            
                //if the relationship exists
                if (relation > 0)
                {
                    //iterate through all points again to change their WP value
                    for (int point = 0; point < num_points; point++)
                    {
                        //record the position of the point
                        vector pos = point(0, "P", point);
    
                        //determine the distance between that point and the seed
                        float distance = distance(pos_seed, pos);

                        //if the distance is within a certain range according to relationship
                        if (((rel_distance*relation - range) < distance) && (distance < (rel_distance*relation + range)))
                        {
                            //determine the wp name    
                            string wp_name = "wp" + itoa(func);
    
                            //change the wp of the chosen voxels
                            float old_wp = point(0, wp_name, point);
                            float wp = old_wp*wp_scale;//pow(10,0.8);
                    
                            //setting new wp
                            setpointattrib(0,wp_name,point,wp,"set");
                        }
                    } 
                }
            }
        }
    }
}
