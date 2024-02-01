import h5py
from sklearn.metrics import rand_score
import sys

fn = sys.argv[1]
gn = sys.argv[2]

eps = int(fn.split("_")[2])
minPts = int(fn.split("_")[4])

labels_pred = h5py.File(fn)['grp']['labels']
labels_true = h5py.File(gn)[f'eps={eps}, minPts={minPts}']['clustering_labels']

print(rand_score(labels_true, labels_pred))

