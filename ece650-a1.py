#!/usr/bin/env python

import re
import sys
import operator
import time
import os
    

class Assignment_1:
    

        def __init__(st):   # defining and initializing the st and self objects 
    
            
            st.street_nodes = {}                # key:value street_name:[street_nodes]
            st.V = {}                           # key:value assigned vertex_id:{vertex coordinates}
            st.E = []                           # elements: connects two vertex ids from V
            st.street_lines = {}                # key:value street_name:(street_lines)
            st.intersections = set()            # tuple of intersecting coordinates
            st.temp_list = set()                # storage for the shortest path (temporary)
            st.V_ids = {}                       # storage to match vertex ID and vertex
            st.maxid = 0                        # the max id for the current vertex
            st.end_re = re.compile("^exit$")    # the regular expression for the pattern of closing the program
            st.error_messages = {               # list of possible errors key:value
                "wrong_input_format"    : "Error: input does not match the required format.\n" , 
                "street_does_not_exist" : "Error: 'c' or 'r' specified for a street that does not exist.\n",
                "insufficient_nodes"    : "Error: 'a' or 'c' should specify at least two nodes.\n",
                "illegal_node_value"    : "Error: the values in coordinates should be only integer or float numbers.\n" ,
                "add_existing_street"   : "Error: 'a' specified an existing street, use 'c' instead.\n",
                "wrong_coordinates"     : "Error: Coordinates specified without right format, there should be no spaces between.\n",
                "system_error"          : "Error: I'm sorry this is an unknown system_error. Restart your program. \n"
            }   

        def get_command(st, command):   # Defining all commands from the standard input

            while(not re.compile('^g$').search(command) and not st.end_re.search(command)):
                if command != "+" and st.command_handler(command):  

                    st.V.clear()
                    del st.E[0:]
                    st.intersections.clear()
                    st.temp_list.clear()
                    st.populate_tree()
    

                try:
                    command = raw_input()
                except EOFError:
                    command = "exit"

            return command
    

        def run_sim(st):

            while(1):
            
                command = '+'
                try:
                    command = st.get_command(command)
                except EOFError:
                    break
                if re.compile('^g$').search(command):
                    st.output_tree()
                elif st.end_re.search(command):
                    break
                else:
                    sys.stderr.write(st.error_messages["system_error"])
    
        def does_street_exist(st, street_name): # Check for existence of the street 

            if street_name in st.street_nodes.keys():
                return True
            else:
                return False
    

        def command_handler(st, command):# Handles all input commands, the exceptions & errors and stores streets as its nodes
            
            ac_command_re = re.compile("^(a|c)\s+(\"\s*[A-Za-z]+\s*[A-Za-z]+\s*[A-Za-z]+\s*[A-Za-z]+\s*\")\s+((\(\s*[-]?[0-9]{1,3}?\s*\,\s*[-]?[0-9]{1,3}?\s*\)+\s*)+)$")
            remv_command_re = re.compile("^r\s(\"(.*)\")$")
            graph_command_re = re.compile('^g$')
            node_re = re.compile("(.*),(.*)\)")
    
            
            if ac_command_re.search(command):
                data = ac_command_re.findall(command)[0]

                action = data[0]
                street_name = data[1].lower()
                nodes = data[2].strip(" ").split("(")
                del nodes[0]
    

                if len(nodes) < 2:
                    #sys.stderr.write(st.error_messages["insufficient_nodes"])
                    return False
    

                nodes_tuple = []
    

                if st.does_street_exist(street_name):
                    if action == 'a':
                        sys.stderr.write(st.error_messages["add_existing_street"])
                        return False
                else:
                    if action == 'c':
                        sys.stderr.write(st.error_messages["street_does_not_exist"])
                        return False
    

                for node in nodes:
                    if not node_re.search(node):
                        sys.stderr.write(st.error_messages["wrong_coordinates"])
                        return False
    

                    node_data = node_re.findall(node)[0]
                    try:
                        node = (float(node_data[0]), float(node_data[1]))
                        nodes_tuple.append(node)
                    except ValueError:
                        sys.stderr.write(st.error_messages["illegal_node_value"])
                        return False
    

                st.street_nodes[street_name] = nodes_tuple
                return True
    

            elif remv_command_re.search(command):

                street_name = remv_command_re.findall(command)[0][0].lower()

                if not st.does_street_exist(street_name):
                    sys.stderr.write(st.error_messages["street_does_not_exist"])
                    return False

                del st.street_nodes[street_name]
                return True

            elif not graph_command_re.search(command):
                sys.stderr.write(st.error_messages["wrong_input_format"])
                return False


        def populate_tree(st): # getting the nodes and the edges

            street_names = st.street_nodes.keys()
    
            # getting the intersections
            for street_i in range(0, len(street_names) - 1):

                for street_j in range (street_i + 1, len(street_names)):
    

                    street_i_name = street_names[street_i]
                    street_j_name = street_names[street_j]
    

                    st.street_lines[street_i_name] = set()
                    st.street_lines[street_j_name] = set()
    

                    street_i_nodes = st.street_nodes[street_i_name]
                    street_j_nodes = st.street_nodes[street_j_name]
    

                    for node_i in range(0, len(street_i_nodes) - 1):
    

                        for node_j in range(0, len(street_j_nodes) - 1):
    

                            line_i = (street_i_nodes[node_i], street_i_nodes[node_i + 1])
                            line_j = (street_j_nodes[node_j], street_j_nodes[node_j + 1])
    

                            st.street_lines[street_i_name].add(line_i)
                            st.street_lines[street_j_name].add(line_j)
    

                            inter_point = st.calculate(line_i, line_j)
                            if inter_point != False:
                                # print point
                                st.intersections.add(inter_point)
    

            # for every line with intersection, separate and store into a temporary list
            if len(st.intersections) > 0:
                st.is_intersected()
    

        def is_intersected(st): # getting E and V
    

            for intersection in st.intersections:
                for street_name in st.street_lines.keys():
                    temp_remove_list = []
                    temp_add_list = []
                    for line in st.street_lines[street_name]:
                        if st.is_point_on_line(intersection, line):
                            (line_0, line_1) = line
                            add_line_0 = (intersection, line_0)
                            add_line_1 = (intersection, line_1)
    

                            temp_remove_list.append(line)
                            temp_add_list.append(add_line_0)
                            temp_add_list.append(add_line_1)
    

                    for remove_line in temp_remove_list:
                        st.street_lines[street_name].remove(remove_line)
                        if remove_line in st.temp_list:
                            st.temp_list.remove(remove_line)
                    
                    for add_line in temp_add_list:
                        st.street_lines[street_name].add(add_line)
                        st.temp_list.add(add_line)
    

            # separating V
            nodes = set()
            edges = set()
            for temp_nodes in st.temp_list:
                edges.add(temp_nodes)
                for node in temp_nodes:
                    if node not in st.V_ids.values():
                        st.maxid += 1
                        st.V_ids[st.maxid] = node
    

            for edge in edges:
                (node_0, node_1) = edge
                node_id_0 = 0
                node_id_1 = 0
                for node_id in st.V_ids.keys():
                    if node_0 == st.V_ids[node_id]:
                        node_id_0 = node_id
                        st.V[node_id_0] = node_0
                    elif node_1 == st.V_ids[node_id]:
                        node_id_1 = node_id
                        st.V[node_id_1] = node_1
                    if node_id_0 and node_id_1:
                        break
                st.E.append((node_id_0, node_id_1))
    

        def is_point_on_line(st, intersection, line): # Checking whether a point is on the line   

            (inter_x, inter_y) = intersection
            (start_x, start_y) = line[0]
            (end_x, end_y) = line[1]
    
            diffrnce_x_inter = inter_x - start_x
            diffrnce_y_inter = inter_y - start_y

            diffrnce_x_line = end_x - start_x;
            diffrnce_y_line = end_y - start_y;
    
            X_product = diffrnce_x_inter * diffrnce_y_line - diffrnce_y_inter * diffrnce_x_line
    
            if abs(X_product) < sys.float_info.epsilon * 100:
                if st.is_between(line, line, inter_x, inter_y):
                    return True
    
            return False
    

        def calculate(st, line_i, line_j): # Getting the result of the tuple of intersecting coordinates
    
            '''
            From ax+by=c ; a=y2-y1 b=x1-x2 c=x1y2-x2y1
            (a1b2-a2b1)x = b2c1 - b1c2
            (a2b1-a1b2)y = a2c1 - a1c2
            '''    

            (B_i, A_i) = tuple(map(operator.sub, line_i[1], line_i[0]))
            (B_j, A_j) = tuple(map(operator.sub, line_j[1], line_j[0]))
    
            B_i = - B_i
            B_j = - B_j
    

            C_i = line_i[0][0] * line_i[1][1] - line_i[0][1] * line_i[1][0]
            C_j = line_j[0][0] * line_j[1][1] - line_j[0][1] * line_j[1][0]
    

            k_y = A_j * B_i - A_i * B_j
            b_y = A_j * C_i - A_i * C_j
    

            k_x = A_i * B_j - A_j * B_i
            b_x = B_j * C_i - B_i * C_j
    

            if k_y == 0 or k_x == 0: # there is no intersection
                return False
            else:
                x = b_x / k_x
                y = b_y / k_y
    

                if st.is_between(line_i, line_j, x, y): # if the intersection is not on extension
                    return x, y
                return False
    

        def is_between(st, line_i, line_j, x, y): # Checking whether the point is between the lines
            max_x_i = max(line_i[0][0], line_i[1][0])
            min_x_i = min(line_i[0][0], line_i[1][0])
    

            max_y_i = max(line_i[0][1], line_i[1][1])
            min_y_i = min(line_i[0][1], line_i[1][1])
    

            max_x_j = max(line_j[0][0], line_j[1][0])
            min_x_j = min(line_j[0][0], line_j[1][0])
    

            max_y_j = max(line_j[0][1], line_j[1][1])
            min_y_j = min(line_j[0][1], line_j[1][1])
    

            if x >= min_x_i and x <= max_x_i and y >= min_y_i and y <= max_y_i:
                if x >= min_x_j and x <= max_x_j and y >= min_y_j and y <= max_y_j:
                    return True
                else:
                    return False
            else:
                return False
    

        def output_tree(st):    

            # sys.stderr.write('V = {\n')
            # for v in st.V.keys():
            #    sys.stderr.write(' ' + str(v) + ": (" + "%.2f" % st.V[v][0] + "," + "%.2f" % st.V[v][1] + ")\n")
            # sys.stderr.write('}\n')

            
            sys.stderr.write("\nV " + str(len(st.V)) + '\n')

            sys.stderr.write('\nE {')
            maxid = 0
            for e in st.E:
                if maxid == len(st.E) - 1:
                    if int(e[0]) != 0 and int(e[1]) != 0:
                        sys.stderr.write('<' + str(int(e[0])) + "," + str(int(e[1])) + '>')
                else:
                    if int(e[0]) != 0 and int(e[1]) != 0:
                        sys.stderr.write('<' + str(int(e[0])) + "," + str(int(e[1])) + '>')
                    sys.stderr.write(',')
                    maxid += 1
    

            sys.stderr.write('}\n')  


def main():
    
    vertex = Assignment_1()
    vertex.run_sim()

    while True:
        line = sys.stdin.readline()
        time.sleep(0.1)
