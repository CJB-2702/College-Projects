'''
ECE 172A, Homework 2 Robot Traversal
Author: regreer@ucsd.edu
For use by UCSD ECE 172A students only.
'''

import numpy as np
import matplotlib.pyplot as plt
from numpy.core.shape_base import _block_format_index

initial_loc = np.array([0,0])
final_loc = np.array([100,100])
sigma = np.array([[50,0],[0,50]])
mu = np.array([[60, 50], [10, 40]])

def f(x, y):
	return ((final_loc[0]-x)**2 + (final_loc[1]-y)**2)/20000 + 10000*(1/(2*np.pi*np.linalg.det(sigma)))*np.exp(-.5*(np.matmul(np.array([x-mu[0,0], y-mu[0,1]]),np.matmul(np.linalg.pinv(sigma), np.atleast_2d(np.array([x-mu[0,0], y-mu[0,1]])).T)))[0]) + 10000*(1/(2*np.pi*np.linalg.det(sigma)))*np.exp(-.5*(np.matmul(np.array([x-mu[1,0], y-mu[1,1]]),np.matmul(np.linalg.pinv(sigma), np.array([x-mu[1,0], y-mu[1,1]])))))

x = np.linspace(0, 100, 100)
y = np.linspace(0, 100, 100)
z = f(x[:,None], y[None,:])
z = np.rot90(np.fliplr(z))

def Q1_1(x,y,z):
	fig = plt.figure()
	ax = plt.axes(projection='3d')
	ax.contour3D(x, y, z, 100)
	ax.set_xlabel('x')
	ax.set_ylabel('y')
	ax.set_zlabel('z')
	ax.set_title('3D Contour')
	plt.show()

def generate_field(x,y,z):
	#put on contour plot
	plt.contour(x, y, z, 25)
	plt.xlabel("x")
	plt.ylabel("y")
	plt.title("2D contour plot")

	#generate gradient plot
	[dy,dx]=np.gradient(z,x,y)
	#generate directional gradient field
	dy=-dy
	dx=-dx
	ey=dy
	ex=dx
	for i in range(100):
		for j in range(100):
			mag=np.sqrt(dx[i][j]*dx[i][j]+dy[i][j]*dy[i][j])
			ex[i][j]=-dx[i][j]/mag
			ey[i][j]=-dy[i][j]/mag
	sv=4
	skip=(slice(None,None,sv),slice(None,None,sv))
	skipxy=slice(None,None,sv)
	#direction gradient
	#plt.quiver(x[skipxy],y[skipxy],ey[skip],ex[skip])

	#regular field
	plt.quiver(x[skipxy],y[skipxy],-dx[skip],-dy[skip])

	#put on star and asterisk
	plt.scatter(initial_loc[0],initial_loc[1],s=1000,c="blue",marker='$*$')
	plt.scatter(final_loc[0],final_loc[1],s=1000,c="red",marker='*')
	plt.show()

def nearestnode(theta):
	pi=3.1415
	step=1
	dblx=0
	dbly=0
	if(np.absolute(theta)<pi/3):
		dblx=step
	if(np.absolute(theta)>pi*2/3):
		dblx=-step
	if(theta>pi/9 and theta<8/9*pi):
		dbly=step
	if(theta<-pi/9 and theta>-8/9*pi):
		dbly=-step
	return[int(dblx),int(dbly)]

def safetyCheck(blx,bly):
	if blx>99:
		blx=99
	if bly>99:
		bly=99
	if blx<0:
		blx=0
	if bly<0:
		bly=0
	return[blx,bly]
		

#create algorithm
def runBug(x,y,z):
	plt.figure(4)
	#generate direction field
	[dy,dx]=np.gradient(z,x,y)
	dy=-dy
	dx=-dx

	#goto starting position
	blx=initial_loc[0].astype(int)
	bly=initial_loc[1].astype(int)
	#remeber where you have been
	past_steps=np.zeros((100,100),int)
	#start moving
	for i in range(200):
		#go in direction field tells you too
		theta=np.arctan2(float(dy[bly][blx]),float(dx[bly][blx]))
		[dblx,dbly]=nearestnode(theta)
		[blx,bly]=safetyCheck((blx+dblx),(bly+dbly))
		#if there is a problem go in a random direction  until you get somewhere new
		while(past_steps[blx][bly]>0):
			theta=np.random.sample()*3.1415*2-3.1415
			[dblx,dbly]=nearestnode(theta)
			[blx,bly]=safetyCheck((blx+dblx),(bly+dbly))
		past_steps[blx][bly]+=1
		#add location to plot
		plt.scatter(blx,bly,s=100,c="green",marker='$*$')

		#if you reach the goal  stop
		if blx+1 ==final_loc[0] and bly+1 ==final_loc[1] :
			print("success")
			break

	#plot stuff
	generate_field(x,y,z)


runBug(x,y,z)