//SHORTEST PATH

//Date: 24-01-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Adapted from work by: Shervin Azadi, Dr.ir. Pirouz Nourian, Hans Hoogenboom
//Purpose: Determines the shortest horizontal path between seeds by intercepting a vertical circulation shaft first

//A: PREPARE NEW ATTRIBUTE TO HOLD THE POINT ID OF THE CLOSEST SHAFT POINT TO ALL START POINT SEEDS

//get number of shafts
int shaft_count = 0;

for (int point=0; point < npoints(0); point++)
{
    if (point(0,"shaft_origins",point) > -1)
    {
        shaft_count = shaft_count+1;
        
    }
}

//iterate through that number
for (int shaft=0; shaft < shaft_count; shaft++)
{
    //preparing attribute name
    string attribute_name = "Shaft" + itoa(shaft);
    
    for (int point=0; point < npoints(0); point++)
    {
        //set default value
        setpointattrib(0,attribute_name,point,-1,"set");
    }
}


//B: FOR ALL START POINT SEEDS, FIND CLOSEST SHAFT AND SET IN NEW ATTRIBUTE

//if its a parent
if (point(0,"parent",@ptnum) > -1)
{
    //closest shaft and min distance
    int closest_shaft = -1;
    float final_distance = 1000000;
    
    //flatten current parent
    vector parent = point(0,"P",@ptnum);
    parent *= set(1,0,1);
    
    //loop through all points in cloud
    for (int point = 0; point < npoints(0); point++)
    {
        //if you find a shaft
        int shaft_count = point(0,"shaft_origins",point);
        
        if (shaft_count > -1)
        {
            
            //shaft position
            vector shaft = point(0,"P",point);
            
            //record distance
            float curr_distance = distance(parent,shaft);
            
            //if curr_distance is less that final_distance, set as new final
            if (curr_distance < final_distance)
            {
                final_distance = curr_distance;
                closest_shaft = shaft_count;
            }
        }
    }
    //set closest shaft to shaft count
    setpointattrib(0,"closest_shaft",@ptnum,closest_shaft,"set");
}


//C: FOR ALL START POINT SEEDS, FIND POINT ID IN THE CLOSEST SHAFT THAT IS ON THE SAME LEVEL AS THE START POINT SEED AND SET IN ATTRIBUTES CREATED IN STEP A

//FOR EACH PARENT
if (point(0,"parent",@ptnum) > -1)
{
    //GET ITS Y VALUE
    vector pos = point(0,"P",@ptnum);
    float y_value = pos.y;
    
    //FOR EACH SHAFT
    for (int point=0; point<npoints(0); point++)
    {
        int shaft_count = point(0,"shaft_origins",point);
        if (shaft_count > -1)
        {
            //preparing attribute name
            string attribute_name = "Shaft" + itoa(shaft_count);
            
            //GET POINT ON SAME YVALUE
            vector pos_1 = point(0,"P",point);
            int IPX = point(0,"IPX",point);
            int IPY = point(0,"IPY",point);
            int IPZ = point(0,"IPZ",point);
            
            pos_1 += set(0,y_value,0);
            int y_val = y_value/3.0;
            
            int point_ID = -2;
            
            //FIND POINT ID OF POINT IN SHAFT
            for (int i=0; i<npoints(0); i++)
            {
                int IPX_1 = point(0,"IPX",i);
                int IPY_1 = point(0,"IPY",i);
                int IPZ_1 = point(0,"IPZ",i);
                
                if (IPX_1 == IPX)
                {
                    if (IPZ_1 == IPZ)
                    {
                        if (IPY_1 == y_val)
                        {
                            point_ID = i;
                        }
                    }
                }
                
            //Setting point
            setpointattrib(0,attribute_name,@ptnum,point_ID,"set");    
            }
        }
    }
}
