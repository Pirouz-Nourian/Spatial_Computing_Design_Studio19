//GROWING

//Date: 24-01-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Adapted from work by: Shervin Azadi, Dr.ir. Pirouz Nourian, Hans Hoogenboom
//Purpose: To grow the initial seed voxels based on data given to it by a .csv file and to give each function a squareness



//user input
float squareness_factor = chf("squareness_factor");

//GET NUMBER OF PARENTS/AGENTS (FUNCTIONS)
int num_agents = npoints(1);

//FOR EVERY PARENT SEED IN THE ENVELOPE
for (int Agent = 0; Agent < num_agents; Agent++)
{
    //prepare wp name
    string wp_name = "wp" + itoa(Agent);
    
    //prepare a list for y-values
    int y_values[];
    
    //get number of children allocated to that parent (agent)
    int num_children = findattribvalcount(0, "point", "parent", Agent); 
    
    //get desired area for that parent (agent)    
    int desired_area = point(1, "area", Agent); 
    
    //get desired number of floors to grow on for that parent (agent)
    int number_of_floors = point(1,"GrowthHeight",Agent);
    
    //IF THE DESIRED AREA (UNITS: NUMBER OF VOXELS) IS GREATER THAN THE NUMBER OF CHILDREN
    if (desired_area > num_children) 
    {
        //CREATE A LIST OF BOUNDARY VOXEL ID POINTS
        int agent_boundary_id[];
        //CREATE A LIST OF BOUNDARY VOXEL WEIGHTED PRODUCTS
        float agent_boundary_wp[];
        
        //get the y-value of the parent (as float and int)
        int seed_parent_id = findattribval(0,"point","Seed",Agent, 0);
        vector pos = point(0,"P",seed_parent_id);
        float y_value = pos.y;
        int seed_IPY = point(0,"IPY",seed_parent_id);
        y_values = point(0,"Y_Values",seed_parent_id);
        
        //add the y-value of the parent to the list of y-values
        append(y_values,seed_IPY);
        
        //FOR EVERY CHILD OF THE PARENT
        for (int Child = 0; Child < num_children; Child++)
        {
            //GET THE POINT ID OF THE CHILD
            int voxel_id = findattribval(0, "point", "parent", Agent, Child);
            //CREATE A LIST OF THE POINT ID'S OF THE NEIGHBOURS OF THE CHILD
            int voxel_neighbours[] = neighbours(0, voxel_id);
            
            //remove neighbours that are too far away
            foreach (int Neigh; voxel_neighbours)
            {
                vector pos_1 = point(0,"P",Neigh);
                float distance = distance(pos_1,pos);
                if (distance > 3)
                {   
                    removevalue(voxel_neighbours,Neigh);    
                }
            }
            
            //FOR EVERY NEIGHBOUR OF THE CHILD
            foreach (int Neigh; voxel_neighbours)
            {   
                //IF THE NEIGHBOUR IS NOT OCCUPIED
                if (inpointgroup(0, "occupied", Neigh) == 0)
                {   
                    //get the y-value of the neighbour (as float and int)
                    vector bound_pos = point(0,"P",Neigh);
                    float y_value_boundary = bound_pos[1];
                    int bound_IPY = point(0,"IPY",Neigh);
                    
                    //get the weighted product of that neighbour
                    float neigh_wp = point(0,wp_name, Neigh);
                    setpointattrib(0,wp_name,Neigh,neigh_wp*squareness_factor,"set");
                                       
                    //IF AGENT DESIRES RANDOM GROWTH (CSV)
                    if (number_of_floors == 0) // csv = 0
                    {
                        //ADD THE NEIGHBOUR'S POINT ID TO THE LIST OF BOUNDARY VOXEL POINT ID'S
                        append(agent_boundary_id, Neigh); 
                        //ADD THE NEIGHBOUR'S WEIGHTED PRODUCT TO THE LIST OF BOUNDARY VOXEL WEIGHTED PRODUCTS
                        append(agent_boundary_wp, neigh_wp);
                    }
                    
                    //IF AGENT DESIRES GROWTH ON 1 VOXEL LAYER (CSV)
                    if (abs(y_value_boundary - y_value) < 0.001 && number_of_floors == 1) //y-values are relatively equal and csv = 1
                    {
                        //ADD THE NEIGHBOUR'S POINT ID TO THE LIST OF BOUNDARY VOXEL POINT ID'S
                        append(agent_boundary_id, Neigh); 
                        //ADD THE NEIGHBOUR'S WEIGHTED PRODUCT TO THE LIST OF BOUNDARY VOXEL WEIGHTED PRODUCTS
                        append(agent_boundary_wp, neigh_wp);
                    }
                    
                    //IF AGENT DESIRES GROWTH ON MORE THAN 1 OR RANDOM VOXEL LAYERS (CSV)
                    if (number_of_floors > 1); // csv > 1
                        //IF THE NEIGHBOUR'S Y-VALUE IS IN THE LIST OF PARENT Y-VALUES
                        if (find(y_values, bound_IPY) > -1)
                        {
                            //ADD THE NEIGHBOUR'S POINT ID TO THE LIST OF BOUNDARY VOXEL POINT ID'S
                            append(agent_boundary_id, Neigh); 
                            //ADD THE NEIGHBOUR'S WEIGHTED PRODUCT TO THE LIST OF BOUNDARY VOXEL WEIGHTED PRODUCTS
                            append(agent_boundary_wp, neigh_wp); 
                        }

                        //OTHERWISE, IF THE LIST OF PARENT Y-VALUES IS 'FULL' (the length of the list is less than the desired number of floors)
                        else if (len(y_values) < number_of_floors)
                        {
                            //ADD THE NEIGHBOUR'S POINT ID TO THE LIST OF BOUNDARY VOXEL POINT ID'S
                            append(agent_boundary_id, Neigh);
                            //ADD THE NEIGHBOUR'S WEIGHTED PRODUCT TO THE LIST OF BOUNDARY VOXEL WEIGHTED PRODUCTS
                            append(agent_boundary_wp, neigh_wp);  
                            //ADD THE NEIGHBOUR'S Y-VALUE TO THE LIST OF PARENT Y-VALUES
                            append(y_values,bound_IPY);
                        }
                }
            }
        }
        
        //Set the maximum as a new child for the parent
        //SORT THE LIST OF BOUNDARY VOXEL WEIGHTED PRODUCTS FROM MAXIMUM TO MINIMUM
        int sorted_indecies[] = reverse(argsort(agent_boundary_wp));
        //SORT THE LIST OF BOUNDARY VOXEL POINT ID'S FROM MAXIMUM TO MINIMUM
        int sorted_boundary_id[] = reorder(agent_boundary_id, sorted_indecies);
        
        //SET THE CHILD WITH THE MAXIMUM WEIGHTED PRODUCT (FIRST INDEX) AS A PARENT AND AS OCCUPIED
        setpointattrib(0, "parent", sorted_boundary_id[0], Agent, "set");
        setpointgroup(0, "occupied", sorted_boundary_id[0], 1, "set");
    }
    //attribute to check amount and value of y-values in list of y-values for that agent
    int seed_parent_id = findattribval(0,"point","Seed",Agent, 0);
    setpointattrib(0,"Y_Values",seed_parent_id,y_values,"set");
}

