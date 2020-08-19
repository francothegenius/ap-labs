def get_length(inputList):
    count = 0
    for elem in inputList:
        if type(elem) == list:  
            count += get_length(elem)
        else:
            count += 1    
    return count

#test cases
print(get_length([1, [2,3]]))
print(get_length([1, [2, [3, 4]]]))
print(get_length([1, [2, [3, [4, [5, 6]]]]]))
print(get_length([]))
