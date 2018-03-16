# dijkstra
# import what i need
import sys, math, copy

# read the input test
Input = sys.stdin.read()

# the init part
def init(init_input):
	global raw_map, startpoint, goalpoint, handle_map, xmax, ymax, wavefront, dic_map, steps, node, step_switch1, step_switch2, same
	# cut the input in to pieces
	Input_split = init_input.split("\n")
	# init the list which will help to get the start point
	start = []
	# init the list which will help to get the goal point
	goal = []
	# init the list which will help to get the start point
	start_split = []
	# init the list which will help to get the goal point
	goal_split = []
	# init the list which will store the start point
	startpoint = []
	# init the list which will store the goal point
	goalpoint = []
	# init the raw map
	raw_map = []
	# init the list which will store the map which has been changed a litte
	handle_map = []
	# init the acitve set
	wavefront = []
	# init the length of map
	xmax = 0
	# init the width of map
	ymax = 0
	# init the step counter
	steps = 0
	# init the node at the moment
	node = 0
	# help to solve the special condition
	# switch1 is about the start point
	step_switch1 = 0
	# swith2 is about the goal point
	step_switch2 = 0
	# switch same will = 1 when start point = goal point
	same = 0
	# a dictionary to store the whole map
	dic_map = {}
	# start to get the map
	for i in Input_split:
		if i == "":
			# just pass
			pass
		# ignore the "$"
		elif i[0] == "$":
			# so just pass
			pass
		# otherwise
		else:
			# get the start position ( raw data )
			if start == []:
				start.append(i)
			# get the goal position ( raw data )
			elif goal == []:
				goal.append(i)
			# get the map ( raw data )
			else:
				raw_map.append(i)
	# change the raw data to an easier form
	# get the start point
	start_split = start[0].split()
	# get the goal point
	goal_split = goal[0].split()
	# change it to the int form
	# get the start point x-axis
	startpoint.append(int(start_split[0]))
	# get the start point y-axis
	startpoint.append(int(start_split[1]))
	# get the goal point x-axis
	goalpoint.append(int(goal_split[0]))
	# get the goal point y-axis
	goalpoint.append(int(goal_split[1]))
	# reverse the raw_map for easier calculate later
	raw_map.reverse()
	
# to determine the size of the map
def sizeofmap():
	global raw_map, ymax, xmax
	# the width  of map
	ymax = len(raw_map)	
	# the length of map
	try:
		xmax = len(raw_map[0])
	except:
		xmax = 0
	
	
# to solve the special condition	
def doornot():
	global startpoint, goalpoint, step_switch1, step_switch2, same	
	# if the start point is out of the map
	if startpoint[0] < 0 or startpoint[0] > xmax - 1 or startpoint[1] < 0 or startpoint[1] > ymax - 1:
		# change the switch1 to 1
		step_switch1 = 1
	# if the start point is in the map
	else:
		# otherwise do not change
		step_switch1 = 0
	# if the goal point is out of map
	if goalpoint[0] < 0 or goalpoint[0] > xmax - 1 or goalpoint[1] < 0 or goalpoint[1] > ymax - 1:
		# change the switch2 to 1
		step_switch2 = 1
	# if the goal point is in the map
	else:
		# otherwise do not change
		step_switch2 = 0
	# if the start point = the goal point
	if startpoint == goalpoint:
		# make the same (switch)  = 1
		same = 1

# to init the map
# handle the raw data of map
def initmap():
	global raw_map, handle_map, xmax, ymax, wall, dic_map, startpoint, goalpoint
	for i in range(ymax):
		# make a temporary list to store the x-axis
		tmp = []
		for j in range(xmax):
			tmp.append(raw_map[i][j])
		# make all x-axis together to get the whole map
		handle_map.append(tmp)
	# do a little change to the map
	for y in range(ymax):
		for x in range(xmax):
			# if there is a wall change it into "inf0"
			# just for easier calculation later
			if handle_map[y][x] == "X":
				#wall.append([x,y])
				dic_map[(x,y)] = ["inf0","inf0"]
			# make the distance of start point to start point as 0
			elif x == startpoint[0] and y == startpoint[1]:
				dic_map[(x,y)] = [0.0,math.sqrt((startpoint[0]-goalpoint[0])**2+(startpoint[1]-goalpoint[1])**2)]
			# let other cell be infinity
			else:
				dic_map[(x,y)] = [float("inf"),math.sqrt((x-goalpoint[0])**2+(y-goalpoint[1])**2)]

# the function to help to print all the map out at last
def out():
	global dic_map, xmax, ymax, step_switch1
	for y in range(ymax-1,-1,-1):
		for x in range(xmax):
			# if the start point is in the map
			if step_switch1 == 0:
				# to print out in a correct form
				if x != xmax - 1:
					print (format(dic_map[(x,y)][0]+dic_map[(x,y)][1],"6.2f"), end="")
				else:
					# to print out in a correct form
					# plus an enter
					print (format(dic_map[(x,y)][0]+dic_map[(x,y)][1],"6.2f"), end="\n")
			# if the start point is out of map
			else:
				# to print out in a correct form
				if x != xmax - 1:
					print (format(float("inf"),"6.2f"), end="")
				# to print out in a correct form
				# plus an enter
				else:
					print (format(float("inf"),"6.2f"), end="\n")

# get (x,y) 's  eight neighbors
def eight_neighbors_init(x,y):
	global dic_map
	neighbor_init = []
	# if (x,y) is on the edge of the map
	# Let them be "", in this case the calculation later will be easier
	# if (x,y) is not on the edge, it will be easy to deal with
	# just append them in to the list
	# it will return neighbor_init
	try:
		# add the data of (x,y-1)
		neighbor_init.append(dic_map[(x,y-1)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x-1,y-1)
		neighbor_init.append(dic_map[(x-1,y-1)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x-1,y)
		neighbor_init.append(dic_map[(x-1,y)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x-1,y+1)
		neighbor_init.append(dic_map[(x-1,y+1)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x,y+1)
		neighbor_init.append(dic_map[(x,y+1)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x+1,y+1)
		neighbor_init.append(dic_map[(x+1,y+1)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x+1,y)
		neighbor_init.append(dic_map[(x+1,y)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	try:
		# add the data of (x+1,y-1)
		neighbor_init.append(dic_map[(x+1,y-1)])
	except:
		# if cannot add that data because of list out of range
		# just add "" is OK
		neighbor_init.append(["",""])
	# return the list we want
	return neighbor_init

# the distance to goal
# x,y means the middle of 8 neighbors
# p means which neighbor
def dis2goal(x,y,p):
	global goalpoint
	# the point is (x,y-1)
	if p == 0:
		d2g = math.sqrt((x-goalpoint[0])**2+(y-1-goalpoint[1])**2)
	# the point is (x-1,y-1)	
	elif p == 1:
		d2g = math.sqrt((x-1-goalpoint[0])**2+(y-1-goalpoint[1])**2)
	# the point is (x-1,y)
	elif p == 2:
		d2g = math.sqrt((x-1-goalpoint[0])**2+(y-goalpoint[1])**2)
	# the point is (x-1,y+1)
	elif p == 3:
		d2g = math.sqrt((x-1-goalpoint[0])**2+(y+1-goalpoint[1])**2)
	# the point is (x,y+1)
	elif p == 4:
		d2g = math.sqrt((x-goalpoint[0])**2+(y+1-goalpoint[1])**2)
	# the point is (x+1,y+1)
	elif p == 5:
		d2g = math.sqrt((x+1-goalpoint[0])**2+(y+1-goalpoint[1])**2)
	# the point is (x+1,y)
	elif p == 6:
		d2g = math.sqrt((x+1-goalpoint[0])**2+(y-goalpoint[1])**2)
	# the point is (x+1,y-1)
	elif p == 7:
		d2g = math.sqrt((x+1-goalpoint[0])**2+(y-1-goalpoint[1])**2)
	# return the value of distance to goal
	return d2g


# to get the distancce of (x,y) 's neighbors
def eight_neighbors(x,y):
	global dic_map, steps
	# set a middle point as (x,y)
	middle_point = dic_map[(x,y)]
	# get the middle point's neighbors
	neighbor = eight_neighbors_init(x, y)
	# to calculate
	for i in range(len(neighbor)):
		# if one of the neighbor is a wall
		if neighbor[i][0] == "inf0":
			# just pass
			pass
		# otherwise
		else:
			# if one of the neighbor is out of map
			if neighbor[i][0] == "":
				# just pass
				neighbor[i][0] = ""
			# otherwise
			# the distance to the middle point is 1 or sqrt(2)
			# after calculation, compare the distance 
			# if it is smaller , change the distance saved before
			elif i % 2 == 0:
				# means the distance to neighbor is 1
				# add stepcounter
				steps += 1
				dis2nei = 1
				# dertermine if we need to change the dis2nei in the dictionary we saved before
				if middle_point[0] + dis2nei + dis2goal(x, y, i) < neighbor[i][0] + neighbor[i][1]:
					# if it's smaller than before 
					# change it
					neighbor[i][0] = middle_point[0] + dis2nei
					neighbor[i][1] = dis2goal(x, y, i)
			else:
				# means the distance to neighbor is sqrt(2)
				# add stepcounter
				steps += 1
				dis2nei = math.sqrt(2)
				# dertermine if we need to change the dis2nei in the dictionary we saved before
				if middle_point[0]  + dis2nei + dis2goal(x, y, i) < neighbor[i][0] + neighbor[i][1]:
					# if it's smaller than before 
					# change it
					neighbor[i][0] = middle_point[0] + dis2nei
					neighbor[i][1] = dis2goal(x, y, i)
	# save the distance of the eight neighbors to the middle point
	# neighbor[0] means (x,y-1)
	dic_map[(x,y-1)] = neighbor[0]
	# neighbor[1] means (x-1,y-1)
	dic_map[(x-1,y-1)] = neighbor[1]
	# neighbor[2] means (x-1,y)
	dic_map[(x-1,y)] = neighbor[2]
	# neighbor[3] means (x-1,y+1)
	dic_map[(x-1,y+1)] = neighbor[3]
	# neighbor[4] means (x,y+1)
	dic_map[(x,y+1)] = neighbor[4]
	# neighbor[5] means (x+1,y+1)
	dic_map[(x+1,y+1)] = neighbor[5]
	# neighbor[6] means (x+1,y)
	dic_map[(x+1,y)] = neighbor[6]
	# neighbor[7] means (x+1,y-1)
	dic_map[(x+1,y-1)] = neighbor[7]
	# return the new dis2neighbor
	return neighbor

def savewhich():
	for i in wavefront:
		count = wavefront.count(k)
		if count == 2:
			obj = wavefront
			same2 = len(wavefront) - 1
			same1 = wavefront.index(obj)

# fuction help to make the frontwave ( active set)
def makewavefront(x,y):
	
	global wavefront, step_switch1, same
	# if the start point = goal point
	# then the wavefront is empty
	if same == 0:
		if step_switch1 == 0:
			# get the 8 neighbors
			neighbor_init = copy.deepcopy(eight_neighbors_init(x, y))
			neighbor = eight_neighbors(x, y)

			# find which distance has been changed
			diff = []
			# if it has been changed, append it into the wavefront
			for i in range(8):

				if neighbor[i][0] != neighbor_init[i][0]:
					diff.append(i)
				else:
					pass
			# this loop is help to dertermine which cell is goint to be added in to the frontwave
			for j in diff:
				# 0 means (x,y-1)
				if j == 0:
					wavefront.append((x,y-1))
				# 1 means (x,y-1)
				elif j == 1:
					wavefront.append((x-1,y-1))
				# 2 means (x,y-1)
				elif j == 2:
					wavefront.append((x-1,y))
				# 3 means (x,y-1)
				elif j == 3:
					wavefront.append((x-1,y+1))
				# 4 means (x,y-1)
				elif j == 4:
					wavefront.append((x,y+1))
				# 5 means (x,y-1)
				elif j == 5:
					wavefront.append((x+1,y+1))
				# 6 means (x,y-1)
				elif j == 6:
					wavefront.append((x+1,y))
				# 7 means (x,y-1)
				elif j == 7:
					wavefront.append((x+1,y-1))
				# because wavefront is an active set, so there can't be 2 same things in it
				# just pop the same thing out
				for k in wavefront:
					# count if the wavefront has more than 2 same things
					count = wavefront.count(k)
					if count == 2:
						# pop the last same thing
						wavefront.pop()
		else:
			wavefront = []
	else:
		wavefront = []

				
# help to find the smallest distance in the wavefront
def findmin():
	global wavefront, dic_map, node
	# do len(wavefront) times
	for j in range(len(wavefront)):
		# why try?
		# because we just need the point in the map
		# we do not need the point out of the map at all
		try:
			if wavefront[j][0] < 0 or wavefront[j][0] >= xmax or wavefront[j][1] < 0 or wavefront[j][1] >= ymax:
				wavefront.remove(wavefront[j])
		except:
			pass
	# to find the smallest distance
	# get the distance and the coordinate
	mindis = dic_map[wavefront[0]][0] + dic_map[wavefront[0]][1]
	minplace = wavefront[0]
	# do len(wavefront) times
	for i in range(len(wavefront)):
		# if there is a smaller distance
		if dic_map[wavefront[i]][0] + dic_map[wavefront[i]][1] < mindis:
			# change the smallest distance
			mindis = dic_map[wavefront[i]][0] + dic_map[wavefront[i]][1]
			# mark the coordirate of this point
			minplace = wavefront[i]
	node = minplace
	# after get the distance and the coordinate
	# remove this node from the wavefront
	wavefront.remove(minplace)

# the last step
# change the "inf0" to the infinity		
def end():
	global dic_map
	for y in range(ymax):
		for x in range(xmax):
			# if find the "inf0"
			if dic_map[(x,y)][0] == "inf0":
				# change them into infinity
				dic_map[(x,y)][0] = float("inf")
				dic_map[(x,y)][1] = float("inf")
	for i in dic_map:
		if dic_map[i][0] == "":
			dic_map[i][0] = float("inf")
			dic_map[i][1] = float("inf")
	#print (dic_map)

# help to find the path from start point to the goal point				
def findpath(x,y):
	global step_switch1, step_switch2
	# if the start point and the goal point are in the map
	if step_switch1 == 0 and step_switch2 == 0:
		# first let the path = the start point
		path = [[x,y]]
		while path[-1] != startpoint:
			inf = 0
			neighbor = eight_neighbors_init(path[-1][0], path[-1][1])
			for i in range(len(neighbor)):
				# if the neighbor is out of map
				# to make the program succeed
				# make them as infinity
				if neighbor[i][0] == "":
					 neighbor[i][0] = float("inf") 
				# count the number of cells which has infinity is list
				if neighbor[i][0] == float("inf"):
					inf += 1
			# if we can not find the way
			# because all the neighbors of the goal point to start point are infinity
			# in this case, break the loop, the path = []
			if inf == 8:
				path = []
				break
			# otherwise
			# find the shortest way from goal to start
			else:
				# print (neighbor)	
				#!!!!!!!!!
				mindis = min(neighbor)[0]+ min(neighbor)[1]
				minplace = neighbor.index(min(neighbor))
				# make the path
				# 0 means the (x,y-1)
				if minplace == 0:
					path.append([path[-1][0],path[-1][1]-1])
				# 1 means the (x-1,y-1)
				elif minplace == 1:
					path.append([path[-1][0]-1,path[-1][1]-1])
				# 2 means the (x-1,y)
				elif minplace == 2:
					path.append([path[-1][0]-1,path[-1][1]])
				# 3 means the (x-1,y+1)
				elif minplace == 3:
					path.append([path[-1][0]-1,path[-1][1]+1])
				# 4 means the (x,y+1)
				elif minplace == 4:
					path.append([path[-1][0],path[-1][1]+1])
				# 5 means the (x+1,y+1)
				elif minplace == 5:
					path.append([path[-1][0]+1,path[-1][1]+1])
				# 6 means the (x+1,y)
				elif minplace == 6:
					path.append([path[-1][0]+1,path[-1][1]])
				# 7 means the (x+1,y-1)
				else:
					path.append([path[-1][0]+1,path[-1][1]-1])
		# reverse it to get the right form 
		path.reverse()
	# if the start point or goal point is out of map
	# path is always empty
	else:
		path = []
	# print the path
	print (path)
			
# main
# init the input
init(Input)
# get the size of map
sizeofmap()
# determine the special condition
doornot()
#print (step_switch1,step_switch2)
# get the init of the map
initmap()
# init the wavefront
makewavefront(startpoint[0], startpoint[1])
# start the loop
# when the wavefront is empty, break
while wavefront != []:
	findmin()
	if node == (goalpoint[0],goalpoint[1]):
		break
	makewavefront(node[0], node[1])
# do the last step
end()	
# print out the map
out()
if same == 0:
	# if the start point is in of the map
	if step_switch1 == 0:
		# do as common
		print ("Found goal in %d steps" % steps)
	# if the start point is out of map
	else:
		# the steps must be 0
		print ("Found goal in %d steps" % 0)
else:
	# the steps must be 0
	print ("Found goal in %d steps" % 0)
# find the shortest path
findpath(goalpoint[0], goalpoint[1])

