For each value of n we generate 100 binary trees of depth n and assign a sudo randomly generated independent standard normal edge weight to ever edge of the trees. 
We then compute the weight of every leaf in the tree as the sum of the edges leading to that leaf.
The maximum edge weight for each tree is then saved.
We output the average of the maximums as well as the variance. 
Several unifomrly selected leaf weights are also presented. 

Due to the computational complexity of this algorithm being particularly bad no useful results are found. 
the upper bound on the variance may only be seen for particularly large n, this cannot be varified via Monte Carlo. 
