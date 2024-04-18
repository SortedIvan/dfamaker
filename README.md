# DFEdit
DFEdit is a desktop tool for creating and testing deterministic finite automaton machines. 
It is an interactive graphical tool that lets you edit, validate and save your DFA's in a JSON format. 
The graphical interface allows for fast edits and the moving of states and state arrows.
Additionally, it comes with an error checker, which indicates whether a DFA is deterministic, labeled correctly, etc.

Currently, the editing space is limited, but will be expanded in a later update.
Dfa maker further comes with its own file system that uses JSON as a way of storing deterministic finite automatons.

The main UI of the DFA editor is a non-resizable screen (as of now), with a very simple structure that is intuitive to use.

![image](https://github.com/SortedIvan/dfamaker/assets/62967263/ba71d5c1-fa4a-4913-b7b4-e7cac6f264ac)

DFEdit features an error window, which indicates whether the DFA is deterministic and if something fails within the application:

![image](https://github.com/SortedIvan/dfamaker/assets/62967263/a34a8843-acf4-415b-bd21-d385d06a40e5)

Another core feature of DFEdit is string validation. This can only be used after the current DFA has passed a sanity check and has been verified to be finite. 
To enter a string, the user may press on the right tab and write out the string as is, followed by enter to validate.

Correct string screen:

![image](https://github.com/SortedIvan/dfamaker/assets/62967263/7ab95f26-fbfd-4b1e-9d1d-dbd984365386)

Incorrect string screen:

![image](https://github.com/SortedIvan/dfamaker/assets/62967263/dd89f6ad-7ccc-4301-8ec6-691348dc2818)

DFEdit also features a simple JSON file system. Currently, it can only save one DFA at a time (work in progress), but this will be changed in the near future to support multiple files and an elaborate file select system.

