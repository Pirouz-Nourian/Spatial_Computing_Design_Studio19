#CONTINUATION OF 5.1 SHORTEST PATH

#D: CREATE THE SHORTEST PATH FROM THE START POINT SEED TO THE CLOSEST SHAFT, THEN FROM THAT SHAFT TO THE END POINT SEED
#IN A PYTHON NODE

#set geo to the geometry connected to the node (connect adjacent pieces)
node = hou.pwd()
geo = node.geometry()

#import the numpy library
import numpy as np
import os
hippath = os.environ['HIP']
#get connection data from csv file
fname = "Function_Relationship_3.csv"
fpath = os.path.join(hippath, fname)
print(fpath)
# fhandle = open(fpath, "r")
connection_weight = np.genfromtxt(fpath, dtype=int, delimiter=",")

#initiate the graph
import networkx as nx 
G = nx.Graph()

#iterate over the points of geo and add them as nodes of the graph
for point in geo.points(): 
    #retrieve the point number
    id = point.number()
    #add the node to the graph (G)
    G.add_node(id)

#iterate over the prims of geo and add them as edges of the graph
for prim in geo.prims():
    #retrieve the prim number (necessary for later)
    id = prim.number()
    #getting the list of the points
    pnts = prim.points()
    #add the edge to the graph (G) by creating it from points in pnts
    G.add_edge(pnts[0].number(),pnts[1].number())

#set seed_ids to the point ids of the points that are not equal to -1 in the attribute "Seed" inside the geo geometry attached to the node
attrib_name = geo.findPointAttrib("parent")
attrib_closest_shaft = geo.findPointAttrib("closest_shaft")

attrib_shaft0 = geo.findPointAttrib("Shaft0")
attrib_shaft1 = geo.findPointAttrib("Shaft1")
attrib_shaft2 = geo.findPointAttrib("Shaft2")
attrib_shaft3 = geo.findPointAttrib("Shaft3")
attrib_shaft4 = geo.findPointAttrib("Shaft4")
attrib_shaft5 = geo.findPointAttrib("Shaft5")

seed_ids = [0 for i in range(13)]
shaft_index = [0 for i in range(13)]
shaft_points = [0 for i in range(13)]

#go through all points and getting the point IDS of parents
for point in geo.points(): 
    
    myseed = point.intAttribValue(attrib_name) 
    myclose_shaft = point.intAttribValue(attrib_closest_shaft)
    
    myShaft0 = point.intAttribValue(attrib_shaft0)
    myShaft1 = point.intAttribValue(attrib_shaft1)
    myShaft2 = point.intAttribValue(attrib_shaft2)
    myShaft3 = point.intAttribValue(attrib_shaft3)
    myShaft4 = point.intAttribValue(attrib_shaft4)
    myShaft5 = point.intAttribValue(attrib_shaft5)

    if myseed != -1:
        #get the point ID
        ptnum = point.number()
        ##add the point ID at index parent
        seed_ids[myseed] = ptnum
        shaft_index[myseed] = myclose_shaft
        
        #make a list of shaft ID
        parent_points = [myShaft0,myShaft1,myShaft2,myShaft3,myShaft4,myShaft5]
        #add list to shaft points
        shaft_points[myseed] = parent_points
        
#group the points that are in the path
path_group = geo.createPointGroup("path")

print("reset")
#looping through the table and finding connections equal to 1
for i, row in enumerate(connection_weight):
    for j, col in enumerate(row):
        if col == 1:
            #setting you start and end nodes
            start_i = seed_ids[i]
            end_f = seed_ids[j]
            #print(start,end)
            
            #get temp end point
            m = shaft_index[i]
            end_i = shaft_points[i][m]
            
            #if there is a point to go to
            if end_i != -2:
                #Part 1
                #find the shortest path using A-star algorithm
                path_nodeid_s = nx.astar_path(G, start_i, end_i)
                #retrieve the point object given the point number
                path_points_s = [geo.points()[id] for id in path_nodeid_s]
                path_group.add(path_points_s)
                
                #get temp start point
                start_f = shaft_points[j][m]
                
                #if there is a point to go to
                if start_f != -2:
                    #Part 2
                    #find the shortest path using A-star algorithm
                    path_nodeid_f = nx.astar_path(G, start_f, end_f)
                    #retrieve the point object given the point number
                    path_points_f = [geo.points()[id] for id in path_nodeid_f]
                    path_group.add(path_points_f)