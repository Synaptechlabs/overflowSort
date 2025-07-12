

import numpy as np
import random

SIZE = 100000
THRESHOLD = 255
sorted_array = []

def overflow_sort(arr, original_vals, indices):
    next_arr = []
    next_vals = []
    next_indices = []

    for i in range(len(arr)):
        doubled = arr[i] * 2
        orig = original_vals[i]
        idx = indices[i]

        if doubled > THRESHOLD:
            # Insert into sorted_array in DESC order by original value, ASC by index
            insert_pos = 0
            while insert_pos < len(sorted_array):
                val_cmp, idx_cmp = sorted_array[insert_pos]
                if orig > val_cmp:
                    break
                elif orig == val_cmp and idx < idx_cmp:
                    break
                insert_pos += 1
            sorted_array.insert(insert_pos, (orig, idx))
        else:
            next_arr.append(doubled)
            next_vals.append(orig)
            next_indices.append(idx)

    if next_arr:
        overflow_sort(next_arr, next_vals, next_indices)

# Generate data
data = np.array([random.randint(0, 255) for _ in range(SIZE)], dtype=np.uint16)
print("Original:", data)

# Begin overflow recursion
overflow_sort(list(data), list(data), list(range(len(data))))

# Extract only the sorted values
final_sorted = [val for val, _ in sorted_array]
print("Sorted by recursive overflow:", final_sorted)
