//PROXIMITY TO N-E-S-W

//Date: 19-12-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Purpose: To create and set normalized attribute values for all voxels, based on position relative to North, East, West and South.

//The following code details the computation required to set attribute values based on their position relative to the North. The code can be very simply adapted to compute values relative to the South, East and West.




//WRANGLER 1

//INPUT 0 = ENVELOPE POINTS
//INPUT 1  = PRIMITIVE NORTH LINE

//RUN OVER DETAIL

//getting end points of line
vector start_point = point(1,"P",0); 
vector end_point = point(1,"P",1); 

//measuring distance between point and closest point on line
float dist = ptlined(start_point,end_point,@P);

//set distance to new attribute
setpointattrib(0,"proximity_north",@ptnum,dist,"set");

//for every voxel, measure the distance to every point in line
//get smallest distance
//set as new attribute

//number of voxel points
int num_v_points = npoints(0);
//number of line points
int num_l_points = npoints(1);

//going through all points
for ( int point = 0; point < num_v_points; point++)
{
    //initialize shortest distance
    float short_dist = 10000000;
    
    //get current point coordinates
    vector point_pos = point(0,"P",point);
    
    //check for distance for all points
    for ( int line_point = 0; line_point < num_l_points; line_point++)
    {
        //get line point coordinates
        vector point_line_pos = point(1,"P",line_point);
        
        //measure distance
        float curr_dist = distance(point_pos,point_line_pos);
        
        //if point smaller, replace shortest distance
        if (curr_dist < short_dist)
        {
            short_dist = curr_dist;
        }
    }
    
    //set point attribute 
    setpointattrib(0,"proximity_path",point,short_dist,"set");
}




//WRANGLER 2

//normalize distance

int num_points = npoints(0);

//set temporary maximum and minimum value
float max_north = point(0,"proximity_north",0);
float min_north = point(0,"proximity_north",0);

//iterate through all points
for (int point = 0; point < num_points; point++)
{
    //get north distance at that point
    float north_value = point(0,"proximity_north",point);
    
    //if larger or smaller than current min and max value, replace
    if (north_value > max_north) max_north = north_value;
    if (north_value < min_north) min_north = north_value;
}

//iterate through all points
for (int point = 0; point < num_points; point++)
{
    //get north distance at that point
    float north_value = point(0,"proximity_north",point);
    
    //fit value between range 0.001-0.99
    north_value = fit(north_value, min_north, max_north, 0.001, 0.999);

    //set new attribute value
    setpointattrib(0,"proximity_north",point,north_value,"set");
}

