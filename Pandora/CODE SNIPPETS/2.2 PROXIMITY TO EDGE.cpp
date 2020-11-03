//PROXIMITY TO EDGE

//Date: 19-12-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Purpose: To create and set normalized attribute values for all voxels, based on their proximity boundary of the voxel envelope.

//INPUT 0 = CONNECT ADJACENCY NETWORK
//RUN OVER DETAILS

//WRANGLER 1
//determine the boundary  voxels, and set their attributes to 1 

int num_points = npoints(0);
addattrib(0, "point", "proximity_edge", 0.0);
int edgepoints[];

//iterate through all of the points
for (int point = 0; point < num_points; point++)
{ 
   //identify neighbours and determine number of neighbours
    int num_neighbours[] = neighbours(0, point);
    int neigh = len(num_neighbours);

    //get y position of voxel
    vector pos = point(0, "P", point);
    float y_value = pos.y;
    
    //if boundary point    
    if (neigh < 6)
        {
        setpointattrib(0, "proximity_edge", point, 1.0, "set");
        //append(edgepoints, voxel_id);
        } 
    //setting voxels that are on ground floor but not on boundary to 0     
    if (neigh < 6 && y_value == 0 && neigh == 5)
        {
        setpointattrib(0, "proximity_edge", point, 0.0, "set");
        }
}
//INPUT 0 = WRANGLER 1
//RUN OVER DETAILS

//WRANGLER 2
//setting distances to interior points

int num_points = npoints(0);
int edgepoints[];

//iterating through all points, adding all edge points to a list
for (int point = 0; point < num_points; point++)
{
    float inside = attrib(0,"point","proximity_edge",point);
    
    if (inside == 1.0)
    {
        append(edgepoints,point);
    }
}

//iterating through all points 
for (int point = 0; point < num_points; point++)
{
    //getting position of point
    float inside = attrib(0,"point","proximity_edge",point);
    vector current_int = attrib(0,"point","P",point);
    
    //if inside
    if (inside == 0.0)
    {
        float temp_dist = 10000000.0;
        
        //looping through edge points
        for (int edge = 0; edge < len(edgepoints); edge++)
        {
            vector current_ext = attrib(0,"point","P",edgepoints[edge]);
            
            if (current_int[1] == current_ext[1])
            {
                float dist = distance(current_int,current_ext);
            
                if (dist < temp_dist)
                {
                temp_dist = dist;
                }
            }
        }
        
        //setting distance to edge
        setpointattrib(0,"proximity_edge",point,temp_dist,"set");
    }
}

//INPUT 0 = WRANGLER 2
//RUN OVER DETAILS

//WRANGLER 3
//inverting values

int num_points = npoints(0);

float max_prox = 0;

//finding the maximum value
//iterating through all points
for (int point = 0; point < num_points; point++)
{
    float inside_prox = attrib(0,"point","proximity_edge",point);
    
    if (inside_prox > max_prox)
    {
        max_prox = inside_prox;
    }
}

//reverting all the values
for (int point = 0; point < num_points; point++)
{
    float inside_dist = attrib(0,"point","proximity_edge",point);
    float new_dist = (max_prox + 1) - inside_dist;

    //setting the distance
    setpointattrib(0,"proximity_edge",point, new_dist);
}
