# -*- coding: utf-8 -*-
"""
Created on Mon Oct  5 23:46:59 2020

@author: max-z
"""
from numpy.random import randint
import math
import torch


#Adapted from C++ code
class Environment():
    def __init__(self, device):
        self.board=[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
        self.score=0
        self.device=device
        for i in range(2):
            self.insert_new_tile()
    def insert_new_tile(self):
        indices=[]
        for i in range(4):
            for j in range(4):
                if self.board[i][j]==0:
                    indices.append([i,j])
        if len(indices)==0:
            return
        index=randint(0, len(indices))
        probability=randint(0,10)
        tile=2
        if probability==0:
            tile=4
        position=indices[index]
        self.board[position[0]][position[1]]=tile
    def move_arr(self, arr, x):
        filtered=[0,0,0,0]
        if x==0:
            is_zero=True
            for i in range(4):
                if arr[i]!=0:
                    is_zero=False
            if is_zero==True:
                return
            count=0
            for i in range(4):
                k=arr[i]
                if k!=0:
                    filtered[count]=k
                    count+=1
            for i in range(3):
                k=filtered[i]
                if k==filtered[i+1]:
                    filtered[i]=k*2
                    filtered[i+1]=0
                    self.score+=k*2
            count=0
            for i in range(4):
                arr[i]=0
                k=filtered[i]
                if k!=0:
                    arr[count]=k
                    count+=1
        if x==1:
            is_zero=True
            for i in range(4):
                if arr[i]!=0:
                    is_zero=False
            if is_zero==True:
                return
            count=0
            for i in reversed(list(enumerate(filtered))):
                k=arr[i[0]]
                if k!=0:
                    filtered[3-count]=k
                    count+=1
            for i in reversed(list(enumerate(filtered))):
                k=filtered[i[0]]
                if k==filtered[i[0]-1]:
                    filtered[i[0]]=k*2
                    filtered[i[0]-1]=0
                    self.score+=k*2
            count=0
            for i in reversed(list(enumerate(filtered))):
                arr[i[0]]=0
                k=filtered[i[0]]
                if k!=0:
                    arr[3-count]=k
                    count+=1
    def move_board(self, x):
        previous=[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
        for i in range(4):
            for j in range(4):
                previous[i][j]=self.board[i][j]
        if x==0:
            column=[0,0,0,0]
            for i in range(4):
                for j in range(4):
                    column[j]=self.board[j][i]
                self.move_arr(column, 0)
                for k in range(4):
                    self.board[k][i]=column[k]
        if x==1:
            column=[0,0,0,0]
            for i in range(4):
                for j in range(4):
                    column[j]=self.board[j][i]
                self.move_arr(column,1)
                for k in range(4):
                    self.board[k][i]=column[k]
        if x==2:
            for i in range(4):
                self.move_arr(self.board[i], 0)
        if x==3:
            for i in range(4):
                self.move_arr(self.board[i], 1)
        if not self.is_identical(previous):
            self.insert_new_tile()
            return True
        return False
    def is_identical(self, arr):
        for i in range(4):
            for j in range(4):
                if arr[i][j]!=self.board[i][j]:
                    return False
        return True
    
    def print_board(self):
        for i in range(4):
            print(self.board[i])
        print()
    def is_done(self):
        for i in range(4):
            if self.is_done_arr(self.board[i])==False:
                return False
        transposed_board=self.clean_transpose()
        for j in range(4):
            if self.is_done_arr(transposed_board[j])==False:
                return False
        return True
    def clean_transpose(self):
        transposed=[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]
        for i in range(4):
            for j in range(4):
                transposed[i][j]=self.board[j][i]
        return transposed
    def is_done_arr(self, arr):
        for i in range(4):
            if arr[i]==0:
                return False
            
        for i in range(3):
            if arr[i]==arr[i+1]:
                return False
        return True
    def get_observation(self):
        observation=torch.zeros(1,16,4,4)
        board=self.board
        for i in range(4):
            for j in range(4):
                if board[i][j]!=0:
                    x=int(math.log2(board[i][j]))
                    observation[0][x-1][i][j]=1
        observation=observation.to(self.device)
        return observation
    

    
    
    
    