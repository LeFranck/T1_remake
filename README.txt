How to run it:

0) Run make in /Program folder
1) If 0) give the error missing gtk, run: apt-get install libgtk-3-dev
2) Pick a file to test from tests folder (The visual representation of the files are in easys, normals, hards, lunatics)
3) Run:   ./solver < file_to_solve.txt | ./watcher 

The program has to connect the nucleus of the same colors through pipes and use the full network of possibles pipes while doing it. 

The solution has two processes
  First:  connect all the nucleus
  Second: Disconnect pipes and reconnect pipes to use the full network, using backtracking
  
