import ctypes


#configure
path_finder = ctypes.CDLL('liba_star_helper.dylib')
class SearchResult(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int), ("y", ctypes.c_int)]
path_finder.find_path.restype=ctypes.POINTER(SearchResult)
path_length = ctypes.c_int() 


# Create a 2D array 
matrix_2d = [[0] * 400 for _ in range(400)] #all zeros initially.
matrix_2d[2][2]=2
matrix_2d[1][2]=2

# Convert the Python list of lists to a ctypes array of arrays
matrix_arr_c_type = (ctypes.c_int * 400) * len(matrix_2d)
matrix_arr_c = matrix_arr_c_type()
for i, subarr in enumerate(matrix_2d):
    matrix_arr_c[i] = (ctypes.c_int * len(subarr))(*subarr)


start_x, start_y = 0, 0
dest_x, dest_y = 2, 3

result_ptr = path_finder.find_path(matrix_arr_c,start_x, start_y, dest_x, dest_y, ctypes.byref(path_length))
if path_length.value >0:
    path = [result_ptr[i] for i in range(path_length.value)]
    for i, point in enumerate(path):
        print(f"step {i+1}: x={point.x}, y={point.y}")
else:
    print("path length is 0")


 
