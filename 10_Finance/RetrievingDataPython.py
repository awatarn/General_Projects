# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
print("+++++googlefinance example+++++")
from googlefinance import getQuotes
import json


symbol = 'PTT'        
print(json.dumps(getQuotes('SET:' + symbol), indent=2)) # for Stock of Thailand, There is a prefix with 'SET:'
print()