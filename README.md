A csv file with sample data can be found in this repository (though the one I trained on is significantly larger), 
along with an executable to generate the data, and a demonstration of the network in demonstration.py. The C++ code for the exe can be found in GameCode.cpp and game.cpp.

I created my dataset by solving the game 2048 with a Monte-Carlo approach (see https://towardsdatascience.com/2048-solving-2048-with-monte-carlo-tree-search-ai-2dbe76894bab). 
I recorded my moves and board positions and then trained a network to classify board positions into movements. 
The network architecture I used was taken from the paper shown via this link: https://www.jstage.jst.go.jp/article/ipsjjip/27/0/27_340/_pdf. 
The authors of that paper used an approach  similar to the one I used but created their data by using bit shift operations and heuristic evaluation. 
Implementing that method was beyond the scope of my project, but it would be a way to improve my array-based approach.
