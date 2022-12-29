# Computer-Networks-Monsoon-2022

## Usage

Step 1 : Compile the files using the command - 

    g++ -std=c++11 main.cpp routing_algo.cpp -o rip

Step 2 : Run the compiled executable file using the command - 

    ./rip < sampleinput.txt

## Input Format

    - The first line of the input file contains the number of nodes in the network.
    - The second line of the input file contains the names of the nodes in the network.
    - The subsequent lines contain the routing entries of connected nodes in the network. in the format- 
        <node_name> <node_ ip> <neighbour_node_ip> <neighbour_node_name> <cost>
    - The last line of the input file should be EOE (End Of Entries).

## Output Format
    
        - The output file contains the routing tables of all the nodes in the network.
        - The output file is in the format - 
            to: <interface_ip> src: <interface_ip> hop: <next_hop_ip> cost: <cost> 
        - The output file is sorted in ascending order of the node names.

## Sample Input 

<img src="https://i.ibb.co/sFv708K/sample-example.png" alt="sample-example" border="0"  style="width:1000px;"><br />

    3
    A B C
    A 10.0.0.1 10.0.0.21 B 1
    B 10.0.0.21 10.0.0.1 A 1
    B 10.0.1.23 10.0.1.3 C 1
    C 10.0.1.3 10.0.1.23 B 1
    EOE

## Sample Output
	 
     A:
	 to: 10.0.0.1 | src: 10.0.0.1 | hop: 10.0.0.1 | cost: 0
	 to: 10.0.0.21 | src: 10.0.0.1 | hop: 10.0.0.21 | cost: 1
	 to: 10.0.1.23 | src: 10.0.0.1 | hop: 10.0.0.21 | cost: 1
	 to: 10.0.1.3 | src: 10.0.0.1 | hop: 10.0.0.21 | cost: 2
	 B:
	 to: 10.0.0.21 | src: 10.0.0.21 | hop: 10.0.0.21 | cost: 0
	 to: 10.0.0.21 | src: 10.0.1.23 | hop: 10.0.0.21 | cost: 0
	 to: 10.0.1.23 | src: 10.0.1.23 | hop: 10.0.1.23 | cost: 0
	 to: 10.0.1.23 | src: 10.0.0.21 | hop: 10.0.1.23 | cost: 0
	 to: 10.0.0.1 | src: 10.0.1.23 | hop: 10.0.0.21 | cost: 1
	 to: 10.0.0.1 | src: 10.0.0.21 | hop: 10.0.0.1 | cost: 1
	 to: 10.0.1.3 | src: 10.0.1.23 | hop: 10.0.1.3 | cost: 1
	 to: 10.0.1.3 | src: 10.0.0.21 | hop: 10.0.1.23 | cost: 1
	 C:
	 to: 10.0.1.3 | src: 10.0.1.3 | hop: 10.0.1.3 | cost: 0
	 to: 10.0.0.21 | src: 10.0.1.3 | hop: 10.0.1.23 | cost: 1
	 to: 10.0.1.23 | src: 10.0.1.3 | hop: 10.0.1.23 | cost: 1
	 to: 10.0.0.1 | src: 10.0.1.3 | hop: 10.0.0.21 | cost: 2

## License
This software is distributed under the Mit License. See the LICENSE file for more details.