# -*- coding: utf-8 -*-
"""
Created on Mon Oct  5 23:33:59 2020

@author: max-z
"""
import torch
import pandas as pd



#Reads data from a given file. 
#The network takes 16 channel input, so the prepare_batch function reads the file and reshapes
#it to a 4d tensor composed of 16 channeled 4 by 4 board representations.
class Dataset():
    def __init__(self, path_observation_file, chunksize, device):
        self.path_observation_file=path_observation_file
        self.nrows=chunksize
        self.data=pd.read_csv(self.path_observation_file, iterator=True, dtype='int64')
        self.device=device
        
    def prepare_batch(self):
        batch=self.data.get_chunk(self.nrows)
        batch = batch.sample(frac=1).reset_index(drop=True)
        targets=torch.LongTensor(batch.iloc[:,256].values).to(self.device)
        if targets.size()[0]<1000:
            observations=torch.FloatTensor(batch.iloc[:, 0:256].values).to(self.device)
            observations=observations.reshape(targets.size()[0],16,4,4)
            return observations, targets, False
        observations=torch.FloatTensor(batch.iloc[:, 0:256].values).to(self.device)
        observations=observations.reshape(1000,16,4,4)
        return observations, targets, True
    
    