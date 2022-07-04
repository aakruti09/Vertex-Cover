def intersect(t1,t2,t3,t4):
      if t1==t2 or  t3==t4 or (t1==t3 and t2==t4) or (t1==t4 and t2==t3):
            return False
      x1=t1[0] ; y1=t1[1]
      x2=t2[0] ; y2=t2[1]
      x3=t3[0] ; y3=t3[1]
      x4=t4[0] ; y4=t4[1]

      #Now checking that point abscissa exist on line or not
      if max(x1,x2)<min(x3,x4):
            x=max(min(x1,x2),min(x3,x4))
            y=min(max(x1,x2),max(x3,x4))
            
      #Line is not an abscissa
      else:
            #Finding slope of both the lines
            if x1-x2!=0:      slopel1=(y1-y2)/(x1-x2)
            else:       slopel1=0
            if x3-x4!=0:      slopel2=(y3-y4)/(x3-x4)
            else:       slopel2=0
            #print('Slope:',slopel1,slopel2)
##            if slopel1==slopel2:
##                  print("No intersection between lines because slopes are same")
##                  return False

            b11=y2-(slopel1*x2)
            b12=y1-(slopel1*x1)
            b21=y4-(slopel2*x4)
            b22=y3-(slopel2*x3)
            #print('Y intersept:',b11,b12,b21,b22)
            if b11!=b12:
                  b11=b12=0
                  x=x1=x2
                  y=(slopel2*x)+b21
            elif b21!=b22:
                  b21=b22=0
                  x=x3=x4
                  y=(slopel1*x)+b11
            else:
                  if slopel1-slopel2!=0:        x=(b21-b11)/(slopel1-slopel2)
                  else:       x=0
                  y=slopel1*x+b11
                  #print(x,y)

      #print('Slope 1 :',slopel1,'\tSlope 2 :',slopel2,'\tb1 :',b11,'\tb2 :',b22)   
      if ((x<max(min(x1,x2),min(x3,x4))) or (x>min(max(x1,x2),max(x3,x4)))):
            #print('No intersection because of x')
            return False
      elif (y>min(max(y1,y2),max(y3,y4))) or (y<max(min(y1,y2),min(y3,y4))):
            #print('No intersection because of y')
            return False
      else:
            inters=(x,y)
            return (t1,t2,t3,t4,inters)

##t=intersect((2, 4), (6, 4), (4, 2), (4, 4))
##print(t)


def makelines(s):
      l=[];l1=[]
      for i in s:
            #print(i)
            for j in range(len(i)-1):
                  #print(i[j])
                  l.append([i[j],i[j+1]])
      return l

def comb(st):
      l=[]
      for i in st:
            #print('Data:',i)
            l.append(i)
      arr=[]

      for i in range(len(l)):
            arr.append([])
            for j in range(len(l[i])-1):
                  arr[i].append([l[i][j] , l[i][j+1]])
      #print(arr)
      #[[[(2, -1), (2, 2)], [(2, 2), (5, 5)], [(5, 5), (5, 6)], [(5, 6), (3, 8)]], [[(4, 2), (4, 8)]], [[(1, 4), (5, 8)]]]

      #arr=[[[(2, -1), (2, 2)], [(2, 2), (5, 5)], [(5, 5), (5, 6)], [(5, 6), (3, 8)]], [[(4, 2), (4, 8)]], [[(1, 4), (5, 8)]]]
      calls=[]
      for i in range(len(arr)):
            for j in range(i+1,len(arr)):
                  #print(arr[i],arr[j])
                  for k in arr[i]:
                        for m in arr[j]:
                              #print(k,m)
                              calls.append([k[0],k[1],m[0],m[1]])
      #print('Calls:',calls)
      return calls                  


def call(calls):
      v=dict() ; cnt=1 ; e=set() ; m=[]
      for i in calls:
            #print(i[0],i[1],i[2],i[3])
            ans=intersect(i[0],i[1],i[2],i[3])
            if ans!=False:
                  e.add((ans[4],ans[0]))
                  e.add((ans[4],ans[1]))
                  e.add((ans[4],ans[2]))
                  e.add((ans[4],ans[3]))
                  if ans[4] not in m:     m.append(ans[4])
                  for j in ans:
                        if j not in v.values():
                              v[cnt]=j
                              cnt+=1
                              #print(v)
      #print('Vertices : ',v)
      #print('Edges:',e,len(e))
      return v,e,m

def initedge(line,inter):
      #print('Intersection:',inter)
      u=[]
      for i in line:
            u.append(i)
            for j in inter:
##                  print(i,j,end=" ")
##                  print(j[0],j[1])
                  x=j[0];y=j[1]
                  x1=i[0][0];y1=i[0][1]
                  x2=i[1][0];y2=i[1][1]
##                  print(x1,y1,x2,y2)
                  if (x1-x2)==0:    sl=0
                  else:       sl=(y1-y2)/(x1-x2)
##                  print('Slope:',sl,end=" ")
##                  m.append(sl)
                  b1=y1-(sl*x1)
                  b2=y2-(sl*x2)
                  if b1!=b2:
##                        print('y:',(sl*x),' b : no b',end=" ")
                        if x==x1 and y>=min(y1,y2) and y<=max(y1,y2):
                              i.append(j)
##                             print(True)
##                        else:
##                              print(False)
                  else:
                        ry=round(y,2)
                        ans=round((sl*x)+b1,2)
                        #print('y : ',ans,' b:',b1,end=" ")
                        if ry==ans:
                              i.append((x,y))
##                              print(True)
##                        else:
##                              print(False)
##      print(u)
      for i in u:
            if len(i)<=2:
                  u.remove(i)
      fe=[]
      for i in u:
            i.sort()
            for j in range(len(i)-1):
                  if ([i[j],i[j+1]] not in fe) and ([i[j+1],i[j]] not in fe) and (i[j] != i[j+1]):
                        fe.append([i[j],i[j+1]])
      #print('Final edges: ',fe)
      return fe
      

def valuekey(v,val):
      for k,v in v.items():
            if v==val:
                  return k
            
def veredge(v,e):
      fe=[]
      for i in e:
            i=list(i)
            #print(i)
            #print(i[0],i[1])
            if (valuekey(v,i[0])!=None) and (valuekey(v,i[1])!=None):
                  fe.append([valuekey(v,i[0]),valuekey(v,i[1])])
      #print("Final edges: ",fe)
      return fe


def graph_print(allstreet):
      #[[(2, -1), (2, 2)], [(2, 2), (5, 5)], [(5, 5), (5, 6)], [(5, 6), (3, 8)], [(4, 2), (4, 8)], [(1, 4), (5, 8)]]
      lines=makelines(allstreet)
      calls_intersect=comb(allstreet)
      vertices,edg,inter_var=call(calls_intersect)
      #edges=makeedges(edg,inter_var)
      edges=initedge(lines,inter_var)
      
      final_edge=veredge(vertices,edges)
      #printing(vertices,final_edge)
      return vertices,final_edge
      
