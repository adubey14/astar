import ctypes
import numpy as np
path_finder = ctypes.CDLL('liba_star_helper.dylib')
path_finder.load_matrix()
class SearchResult(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int), ("y", ctypes.c_int)]
 
path_finder.find_path.restype=ctypes.POINTER(SearchResult)

start_x, start_y = 0, 0
dest_x, dest_y = 200, 300
path_length = ctypes.c_int()
 
result_ptr = path_finder.find_path(start_x, start_y, dest_x, dest_y, ctypes.byref(path_length))
if path_length.value >0:
    path = [result_ptr[i] for i in range(path_length.value)]

    for i, point in enumerate(path):
        print(f"step {i+1}: x={point.x}, y={point.y}")
else:
    print("path length is 0")

 
