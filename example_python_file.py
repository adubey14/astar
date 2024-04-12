import ctypes
import os

N=400

#configure
# if on linux change this to .so which will be compiled for you.
if os.path.exists('./liba_star_helper.dylib'):
    libname="./liba_star_helper.dylib"
else:
    libname="./liba_star_helper.so"

path_finder = ctypes.CDLL(libname)


class SearchResult(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int), ("y", ctypes.c_int)]
path_finder.find_path.restype=ctypes.POINTER(SearchResult)
path_length = ctypes.c_int() 


# Create a 2D array 
matrix_2d = [[0] * N for _ in range(N)] #all zeros initially.

#define obstacles
matrix_2d[2][2]=2
matrix_2d[1][2]=2

# Convert the Python list of lists to a ctypes array of arrays
matrix_arr_c_type = (ctypes.c_int * N) * len(matrix_2d)
matrix_arr_c = matrix_arr_c_type()
for i, subarr in enumerate(matrix_2d):
    matrix_arr_c[i] = (ctypes.c_int * len(subarr))(*subarr)


#change startting and destination position
start_x, start_y = 0, 0
dest_x, dest_y = 0, 1
import random

NUM_LOCATIONS=1000

random_start = [(random.randint(0, N-1), random.randint(0, N-1)) for _ in range(NUM_LOCATIONS)]
random_end = [(random.randint(0, N-1), random.randint(0, N-1)) for _ in range(NUM_LOCATIONS)]

for i in range(NUM_LOCATIONS):
    result_ptr = path_finder.find_path(matrix_arr_c,random_start[i][0], random_start[i][1], random_end[i][0], random_end[i][1], ctypes.byref(path_length))
    if path_length.value >0:
        path = [result_ptr[i] for i in range(path_length.value)]
        print(f'{i}:{path_length.value}')
        #for i, point in enumerate(path):
            #print(f"{i+1}:({point.x},{point.y})",end="; ")
    else:
        print("path length is 0")


#test this function by
# cmake .
# make
# time python example_python_file.py  
# divide real answer by NUM_LOCATIONS which is 1000
