//PROXIMITY TO NOISE

//Date: 19-12-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Purpose: To create and set normalized attribute values for all voxels, based on their proximity to the noisiest area of the voxel envelope.

//INPUT 0 = CONNECT ADJACENCY NETWORK
//RUN OVER DETAILS

//WRANGLER 1
//determine the noisiest voxels, and set their attributes to 1 

int num_points = npoints(0);

//add a new attribute
addattrib(0, "point", "proximity_noise", 0.0);

//initialize empty list for edge points 
int edgepoints[];

//iterate through all points
for (int point = 0; point < num_points; point++)
{ 
    //identify neighbours and determine number of neighbours
    int num_neighbours[] = neighbours(0, point);
    int neigh = len(num_neighbours);

    //get y position of voxel
    vector pos = point(0, "P", point);
    float y_value = pos.y;
    
    //if voxel on boundary, and on first 3 floors, it is a noisy neighbour, set attribute value to 1
    if (neigh < 6 && y_value < 15)
        {
        setpointattrib(0, "proximity_noise", point, 1.0, "set");
        }
    //setting voxels that are on ground floor but not on boundary to 0 
    if (neigh < 6 && y_value == 0 && neigh == 5)
        {
        setpointattrib(0, "proximity_noise", point, 0.0, "set");
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
    float inside = attrib(0,"point","proximity_noise",point);
    if (inside == 1.0)
    {
        append(edgepoints,point);
    }
}

//iterating through all points 
for (int point = 0; point < num_points; point++)
{
    //getting position of point
    float inside = attrib(0,"point","proximity_noise",point);
    vector current_int = attrib(0,"point","P",point);
    
    //if inside
    if (inside == 0.0)
    {
        float temp_dist = 10000000.0;
        
        //looping through edge points
        for (int edge = 0; edge < len(edgepoints); edge++)
        {
            vector current_ext = attrib(0,"point","P",edgepoints[edge]);
                float dist = distance(current_int,current_ext);
            
                if (dist < temp_dist)
                {
                temp_dist = dist;
                }
        }
        //setting distance to edge
        setpointattrib(0,"proximity_noise",point,temp_dist,"set");
    }
}


//INPUT 0 = WRANGLER 2
//RUN OVER DETAILS

//WRANGLER 3
//inverting values

float max_prox = 0;

//finding the maximum value
//iterating through all points
for (int point = 0; point < num_points; point++)
{
    float inside_prox = attrib(0,"point","proximity_noise",point);
    
    if (inside_prox > max_prox)
    {
        max_prox = inside_prox;
    }
}

//reverting all the values
for (int point = 0; point < num_points; point++)
{
    float inside_dist = attrib(0,"point","proximity_noise",point);
    float new_dist = (max_prox + 1) - inside_dist;

    //setting the distance
    setpointattrib(0,"proximity_noise",point, new_dist);
}
