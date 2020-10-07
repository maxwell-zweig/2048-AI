# -*- coding: utf-8 -*-
"""
Created on Mon Oct  5 23:40:26 2020

@author: max-z
"""
import torch
import torch.nn as nn
import torch.nn.functional as F


#Creates a network with 5 convolutional layers, followed by a fully connected layer. 
#1 layer of zero-padding is applied to the right and bottom side of the board.
class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1=nn.Conv2d(16, 256, (2,2), stride=1, padding=0)
        self.conv2=nn.Conv2d(256, 256, (2,2), stride=1, padding=0)
        self.conv3=nn.Conv2d(256, 256, (2,2), stride=1, padding=0)
        self.conv4=nn.Conv2d(256, 256, (2,2), stride=1, padding=0)
        self.conv5=nn.Conv2d(256, 256, (2,2), stride=1, padding=0)
        self.fc1=nn.Linear(4096,4, bias=False)
        self.padding=nn.ZeroPad2d((0,1,0,1))
        
    def forward(self, x):
        x=self.padding(x)
        x=self.conv1(x)
        x=F.relu(x)
        
        x=self.padding(x)
        x=self.conv2(x)
        x=F.relu(x)
        
        x=self.padding(x)
        x=self.conv3(x)
        x=F.relu(x)

        x=self.padding(x)
        x=self.conv4(x)
        x=F.relu(x)

        x=self.padding(x)
        x=self.conv5(x)
        x=F.relu(x)
        
        x=torch.reshape(x,(-1,1,4096))
        x=self.fc1(x)
        sm=nn.Softmax(dim=-1)
        return sm(x)
    
    