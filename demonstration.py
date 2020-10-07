# -*- coding: utf-8 -*-
"""
Created on Mon Oct  5 23:49:56 2020

@author: max-z
"""



import torch
import torch.optim as optim
import torch.nn as nn
import matplotlib.pyplot as plt

from net import Net
from data_handler import Dataset
from environment import Environment

#Choose GPU or CPU, need to have Cuda
if torch.cuda.is_available():
    device=torch.device('cuda:0')
    print('Running on GPU')
else:
    device=torch.device('cpu')
    print('Running on CPU')
    
#Plays 5 sample games
def play_games(net: Net):
    for i in range(5):
        env=Environment(device)
        while not env.is_done(): 
            observation=env.get_observation()
            weights=net(observation).flatten()
            #Occasionally the network chooses the same move repeatedly.
            #In this case, the probability of that move is set to zero, and a different move is picked.
            while True:
                action=torch.multinomial(weights, 1)
                if env.move_board(action):
                    break
                weights[action]=0
        env.print_board()
        
        
def train(net: Net):
    optimizer=optim.Adam(params=net.parameters(), lr=0.001)
    objective=nn.CrossEntropyLoss()
    dataset=Dataset(r'C:\Users\max-z\source\repos\GameCode\GameCode\train_data.csv', 1000, device) 
    observations, targets, is_full=dataset.prepare_batch()
    count=0
    losses=[]
    while is_full:
        count+=1
        optimizer.zero_grad()
        action_probs=net(observations)
        action_probs=action_probs.squeeze()
        loss=objective(action_probs, targets)
        loss.backward()
        optimizer.step()
        observations, targets, is_full=dataset.prepare_batch()        
        losses.append(loss.item())
        print(count)
    plt.plot(losses)
    plt.show()
    torch.save(net.state_dict(), 'net_params.pt') 



net=Net().to(device)
play_games(net)
if input("Enter 'True' to train, or 'False' to load: ")=='True':
    train(net)
else:
    net.load_state_dict(torch.load('net_params.pt'))
play_games(net)

        
        
        
        
        
        
        