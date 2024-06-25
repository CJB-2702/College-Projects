'''
ECE 172A, Homework 2 Maze Pathfinding
Author: regreer@ucsd.edu
Maze generator adapted from code by Łukasz Nojek
For use by UCSD ECE 172A students only.
'''

import matplotlib.pyplot as plt 
import numpy as np

def draw_path(final_path_points, other_path_points):
	'''
	final_path_points: the list of points (as tuples or lists) comprising your final maze path. 
	other_path_points: the list of points (as tuples or lists) comprising all other explored maze points. 
	(0,0) is the start, and (49,49) is the goal.
	Note: the maze template must be in the same folder as this script.
	'''
	im = plt.imread('172maze2021.png')
	x_interval = (686-133)/49
	y_interval = (671-122)/49
	plt.imshow(im)
	fig = plt.gcf()
	ax = fig.gca()
	circle_start = plt.Circle((133,800-122), radius=4, color='lime')
	circle_end = plt.Circle((686, 800-671), radius=4, color='red')
	ax.add_patch(circle_start)
	ax.add_patch(circle_end)
	
	for point in other_path_points:
		if not (point[0]==0 and point[1]==0) and not (point[0]==49 and point[1]==49):
			circle_temp = plt.Circle((133+point[0]*x_interval, 800-(122+point[1]*y_interval)), radius=4, color='blue')
			ax.add_patch(circle_temp)
	
	for point in final_path_points:
		if not (point[0]==0 and point[1]==0) and not (point[0]==49 and point[1]==49):
			circle_temp = plt.Circle((133+point[0]*x_interval, 800-(122+point[1]*y_interval)), radius=3, color='yellow')
			ax.add_patch(circle_temp)
	plt.show()
	#for point in final_path_points:   
	### Your Work Below:


import pickle
the_maze=[]
with open('172maze2021.p','rb') as file:
    while True:
        try:
            the_maze.append(pickle.load(file))
        except EOFError:
            break
the_maze=the_maze[0]

def sense_maze(pos, maze):
	options=[]
	surroundings=maze[(pos[0],pos[1])]
	#print(surroundings)
	if surroundings[1]:#east  	-->
		options.append([pos[0]+1,pos[1]])
	if surroundings[0]:#north	^^^
		options.append([pos[0],pos[1]+1])
	if surroundings[3]:#west	<--
		options.append([pos[0]-1,pos[1]])
	if surroundings[2]:#south	vvv
		options.append([pos[0],pos[1]-1])
	return options


def depth_first_search():
	goal=[49,49]
	start=[0,0]
	#1 initialise empty stack
	#2initialise Nx1 array to keep track of visited nodes
	visited=[]
	#3initialise an Nx1 array of zeros for parent nodes
	#push starting index into stack
	the_stack=[start]
	#while the stack is not empty && you have not reached your goal do
	while the_stack and not (goal in the_stack):
		#pop the first index, i, from the stack
		pos=the_stack.pop()
		#if we haven’t visited the node at i then
		if not(pos in visited):
			#mark it as visited (visited(i) = 1)
			the_stack.append(pos)
			visited.append(pos)
			#get the neighbors of node i (neighbors = sense maze(i, data))
			neighbors= sense_maze(pos, the_maze)
			for neighbor in neighbors:
				#push the neighbor onto the stack
				the_stack.append(neighbor)
				#set the parent of the neighbor to the current node
	if not the_stack:
		print("error goal not reached")
	return the_stack, visited

def breadth_first_search():
	goal=[49,49]
	start=[0,0]
	the_queue=[start]
	visited=[]
	Kchild_Vparent={}
	while the_queue and not (goal in the_queue):
		pos=the_queue.pop(0)
		if not(pos in visited):
			neighbors= sense_maze(pos, the_maze)
			for neighbor in neighbors:
				the_queue.append(neighbor)
				visited.append(pos)
				if not((neighbor[0],neighbor[1]) in Kchild_Vparent):
					Kchild_Vparent[(neighbor[0],neighbor[1])]=pos
	if not the_queue:
		print("error goal not reached")
	path=[goal]
	pos=goal
	while not(start in path):
		pos=Kchild_Vparent[(pos[0], pos[1])]
		path.append(pos)

	return path, visited

[path, visited]=depth_first_search()
draw_path(path, visited)
[path, visited]=breadth_first_search()
draw_path(path, visited)