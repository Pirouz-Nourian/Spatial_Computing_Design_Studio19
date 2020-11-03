MASSING ENVELOPE

//Date: 24-01-2019
//Authors: Jolt Wiersma, Michelle A. Menkiti, Arthur Masure
//Adapted from work by: Shervin Azadi, Dr.ir. Pirouz Nourian, Hans Hoogenboom
//Purpose: Merging voxel values from Obscurity and Visibility analysis to create one aggregated volume, and solar envelope




//WRANGLER 1
//Normalizing all incoming values

//INPUT 0 = OBSCURITY OF SKY
//INPUT 1  = OBSCURITY OF SUN
//INPUT 2 = VISIBILITY OF SKY
//INPUT 3  = VISIBILITY OF SUN

//RUN OVER DETAIL

int num_points = npoints(0);

float max_vsun = point(2,"visibility_sun",0);
float max_vsky = point(3,"visibility_sky",0);
float max_osky = point(0,"obscurity_sky",0);
float max_osun = point(1,"obscurity_sun",0);

float min_vsun = point(2,"visibility_sun",0);
float min_vsky = point(3,"visibility_sky",0);
float min_osky = point(0,"obscurity_sky",0);
float min_osun = point(1,"obscurity_sun",0);

for (int point = 0; point < num_points; point++)
{
    float vsun_value = point(2,"visibility_sun",point);
    float vsky_value = point(3,"visibility_sky",point);
    float osky_value = point(0,"obscurity_sky",point);
    float osun_value = point(1,"obscurity_sun",point);
    
    if (vsun_value > max_vsun) max_vsun = vsun_value;
    if (vsky_value > max_vsky) max_vsky = vsky_value;
    if (osky_value > max_osky) max_osky = osky_value;
    if (osun_value > max_osun) max_osun = osun_value;
    if (vsun_value < min_vsun) min_vsun = vsun_value;
    if (vsky_value < min_vsky) min_vsky = vsky_value;
    if (osky_value < min_osky) min_osky = osky_value;
    if (osun_value < min_osun) min_osun = osun_value;

}

for (int point = 0; point < num_points; point++)
{
    float vsun_value = point(2,"visibility_sun",point);
    float vsky_value = point(3,"visibility_sky",point);
    float osky_value = point(0,"obscurity_sky",point);
    float osun_value = point(1,"obscurity_sun",point);
    
    vsun_value = fit(vsun_value, min_vsun, max_vsun, 0.001, 0.999);
    vsky_value = fit(vsky_value, min_vsky, max_vsky, 0.001, 0.999);
    osky_value = fit(osky_value, min_osky, max_osky, 0.001, 0.999);
    osun_value = fit(osun_value, min_osun, max_osun, 0.001, 0.999);

    //setting normalized values
    setpointattrib(2,"visibility_sun",point,vsun_value,"set");
    setpointattrib(3,"visibility_sky",point,vsky_value,"set");
    setpointattrib(0,"obscurity_sky",point,osky_value,"set");
    setpointattrib(1,"obscurity_sun",point,osun_value,"set");
}




//WRANGLER 2
//Normalizing all incoming values

//INPUT 0 = WRANGLER 1

//RUN OVER DETAIL

//getting inputs from sliders
float O_sky = ch("obscurity_sky"); 
float O_sun = ch("obscurity_sun");
float V_sun = ch("visibility_sun"); 
float V_sky = ch("visibility_sky"); 

//going through each input, and changing the value
//multiplying the normal values by the attribute values

int points = npoints(0);

for (int point = 0; point < points; point++)
{    
    //get attribute, divide it, replace it
    
    //Obscurity_sky
    float value1 = getattrib(0,"point","obscurity_sky",point,1);
    value1 = pow(value1, O_sky);
    //setpointattrib(0,"obscurity_sky",point,0,value1,"set");
    
    //Obscurity_sun
    float value2 = getattrib(0,"point","obscurity_sun",point,1);
    value2 = pow(value2, O_sun);
    //setpointattrib(0,"obscurity_sun",point,0,value2,"set");
    
    //Visibility_sun
    float value3 = getattrib(0,"point","visibility_sun",point,1);
    value3 = pow(value3, V_sun);
    //setpointattrib(0,"visibility_sun",point,0,value3,"set");
    
    //Visibility_sky
    float value4 = getattrib(0,"point","visibility_sky",point,1);
    value4 = pow(value4, V_sky);
    //setpointattrib(0,"visibility_sky",point,0,value4, "set");
    
        //total
    float total = value1 * value2 * value3 * value4;
    
    //setting that value
    setpointattrib(0,"full_visibility",point,total,"set");
}    




//WRANGLER 3
//Normalizing all incoming aggregate values

//INPUT 0 = WRANGLER 2

//RUN OVER DETAIL

int num_points = npoints(0);

//setting temp max and min
float max_total = point(0,"full_visibility",0);
float min_total = point(0,"full_visibility",0);

//iterating through all points
for (int point = 0; point < num_points; point++)
{
    float total_value = point(0,"full_visibility",point);
    
    //adjusting values if new min and max are found
    if (total_value > max_total) max_total = total_value;
    if (total_value < min_total) min_total = total_value;
}

//iterating points
for (int point = 0; point < num_points; point++)
{
    float total_value = point(0,"full_visibility",point);
    
    total_value = fit(total_value, min_total, max_total, 0.001, 0.999);

    //setting new attribute
    setpointattrib(0,"full_visibility",point,total_value,"set");
}




//WRANGLER 4
//Voxel removal

//INPUT 0 = NORMALIZED VOXELS

//RUN OVER POINTS

//removing points if they are over a certain threshold
if (@full_visibility > chf("threshold")) 
{
    removepoint(0, @ptnum);
}






