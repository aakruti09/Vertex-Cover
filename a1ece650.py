#!/usr/bin/env python3
import sys
from intersect_point_new import graph_print
import re
# YOUR CODE GOES HERE

def in_tuples(c):
      try:
            t=[]
            ns=c.strip(' ')
            if ns[-1]!=")":
                  #print('Error: Invalid tuple value')
                  return False
            elif ns[0]!="(":
                  #print('Error: Invalid tuple value')
                  return False
            else:                        
                  ns=c.split(")")
                  del ns[-1]
                  #print('ns',ns)
                  for i in ns:
                        i.strip(' ')
                        if i!="":
                              i=i.strip(' ')
                              i=i.strip('(')
                              #print('i',i)
                              i1,i2=i.split(',',1)
                              t.append((int(i1),int(i2)))
                        else:
                              #print('Error: Invalid tuple valuesss')
                              return False
                  #print(t)
            return t
            
      except:
            #print('Error: Invalid tuple values')
            return False


def overlapping(d):
      nd=dict()
      ckd=[] 
      for k,v in d.items():
            l=list(v)
            l.reverse()
            if (v not in ckd) and (l not in ckd):
                  nd[k]=v
                  ckd.append(v)
      #print(nd)
      return nd

def printing(v,e):
      print('V = {')
      for i in v:
            vl=list(v[i])
            if (type(vl[0])==float) or (type(vl[1])==float):
                  vl[0]=round(vl[0],2)
                  vl[1]=round(vl[1],2)
                  print("  {}:".format(i),end="")
                  print("  ({},{})".format(vl[0],vl[1]))
            else:
                  print("  {}:".format(i),end="")
                  print("  ({},{})".format(vl[0],vl[1]))
      print('}')

      print('E = {')
      if len(v)!=0:
            for i in range(len(e)-1):
                  print('  <{},{}>,'.format(e[i][0],e[i][1]))
            print('  <{},{}>'.format(e[-1][0],e[-1][1]))
      print('}')

      
def main():
      # YOUR MAIN CODE GOES HERE             
      # sample code to read from stdin.
      # make sure to remove all spurious print statements as required by the assignment
      coor=[] ; v=dict() ; e=[] ; splited=[] ; allstreet=dict() ; reg='[A-Za-z ]+$'
      while True:
            #print('Enter command: ')
            line = sys.stdin.readline()
            if line == "":      break

            #Input Splitting
            try:
                  line=line.strip('\n')
                  line=line.strip(' ')
                  cmd=line[0]
                  if cmd=="a" or cmd=="c":
                        if line[1] !=" ":
                              print('Error: Invalid command format, should have space after command')
                              continue
                        l1=line[1:].strip(" ")
                        #print('l1:',l1)
                        if l1[0]!='"':
                              print('Error: Street name should be specified in "inverted comma" ')
                              continue
                        l2=l1[1:]
                        name,c=l2.split('" ',1)

                        #Checking name contains alphabet and space only
                        test=bool(re.match(reg,name))
                        if test==False:
                              print('Error: Street name should only contain alphabets and space')
                              continue
                        #print('Name:',name)

                        #Checking tuple values
                        cor=in_tuples(c)
                        if cor==False:
                              print('Error: Invalid coordinates value')
                              continue
                              
                        #else:       print('Coordinates:',cor)
                        
                  elif cmd=="r":
                        l1=line[1:].strip(" ")
                        if l1[0]!='"' or l1[-1]!='"':
                              print('Error: Invalid r command format')
                              continue
                        else:       name=l1.strip('"')
                        
                  elif cmd=="g":
                        l1=line[1:].strip(" ")
                        if len(l1) > 0:
                              print('Error: Too many arguments for g')
                              continue
                  else:
                        print('Error: Invalid Command')
                        continue
                        
            except:
                  print('Error: Invalid command')
                  continue

            
            if cmd=="a":
                  aflag=True
                  for akey in allstreet.keys():
                        if name.lower()==akey.lower():
                              print('Error: The street name already exist')
                              aflag=False
                              break
                  if aflag:
                        allstreet[name]=cor
                        
            elif cmd=="r":
                  rflag=True
                  for ckey in allstreet.keys():
                        if name.lower()==ckey.lower():
                              allstreet.pop(ckey)
                              rflag=False
                              break
                  if rflag:
                        print("Error: 'r' specified for a street that does not exist.")
                        continue
            elif cmd=="c":
                  cflag=True
                  for ckey in allstreet.keys():
                        if name.lower()==ckey.lower():
                              allstreet[ckey]=cor
                              cflag=False
                              break
                  #print('All street :',allstreet)
                  if cflag:
                        print("Error: 'c' specified for a street that does not exist.")
                        continue
            elif cmd=="g":
                  gallstreet=overlapping(allstreet)
                  #print("All street:",allstreet)
                  v,e=graph_print(gallstreet.values())
                  #print('Returned v:',v)
                  printing(v,e)
      
if __name__ == "__main__":
      main()

