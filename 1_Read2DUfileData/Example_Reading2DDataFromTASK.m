%% Example of calling Reading2DDataFromTASK.m
clc
close all
clear
%%
DirectoryContainingData = sprintf('/Users/vitreloy/git2017/TestScripts/1_Read2DUfileData');
File2dName = sprintf('data_RT1.txt');
DepVarLabel = 'TE';
TimeQuery = 99.15;
Option = 1;

[TimeMatch,rho,Y] = Reading2DDataFromTASK(DirectoryContainingData,File2dName,DepVarLabel,TimeQuery,Option);

figure();
temp1 = sprintf('Time %.4f s',TimeMatch);
plot(rho,Y,'ro')
title(temp1)
ylabel(DepVarLabel);
xlabel('rho');