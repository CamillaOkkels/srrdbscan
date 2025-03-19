import dbscan_srr as dbscan
import numpy as np
from sklearn.datasets import make_blobs
from sklearn.cluster import DBSCAN
from sklearn.metrics import adjusted_rand_score, normalized_mutual_info_score

# create 5 clusters
X,y = make_blobs(n_samples=10_000, n_features=100, centers=5, random_state=0)
eps = 15
minPts = 100

# run our dbscan implementation to retrieve cluster labels
srr = dbscan.SRR()

labels = srr.fit_predict(X, 
    0.1, # probability of missing a point
    1, # memory constraint in GB
    "test", # name of file for some default output
    1, # number of threads
    -1, # use best level dynamically
    1,  # shrinkage factor to avoid looking at all repetitions in a level
    eps, 
    minPts, 
    1.0) # the approximation factor

# print some statistics
print(srr.statistics())

# compute exact groundtruth labels
gt_labels = DBSCAN(eps=eps, min_samples=minPts).fit(X).labels_

# report on quality score compared to exact DBSCAN, and the actual groundtruth labels
print("ARI compared to DBSCAN", adjusted_rand_score(gt_labels, labels), " NMI: ", normalized_mutual_info_score(gt_labels, labels))
print("ARI compared to groundtruth labels:", adjusted_rand_score(y, labels), " NMI: ", normalized_mutual_info_score(y, labels))



