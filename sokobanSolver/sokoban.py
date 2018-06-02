#! /usr/bin/python3

import sys
import copy
import queue
import datetime

charmap = {"all":set("# .@+$*"), "pwall":set("#"), "pgoal":set(".+*"), "pbox":set("$*"), "pman":set("@+")}
dirlist = ['u', 'd', 'l', 'r']
dirrevr = {'u':'d', 'd':'u', 'l':'r', 'r':'l'}

prow    = 0
pcol    = 0
pwall   = 0
pgoal   = 0
pbox    = 0
pman    = 0

pbase   = 0
pspace  = 0
frame   = {'u':0, 'd':0, 'l':0, 'r':0}

def getXSB():
    if(len(sys.argv) != 2):
        print("Usage: ./sokoban [inputFile]")
        exit(1)

    fh = open(sys.argv[1], "r")

    mapset = []
    xsb    = ""

    for line in fh:
        if(set(line.strip('\n')).issubset(charmap["all"]) and len(line.strip('\n'))):
            xsb += line
        if(len(xsb) and not len(line.strip('\n'))):
            mapset.append(xsb)
            xsb = ""

    fh.close()
    return mapset

def getSize(xsb):
    prow = xsb.count('\n')
    pcol = 0
    cnt  = 0
    for c in xsb:
        if(c == '\n'):
            pcol = max(pcol, cnt)
            cnt  = 0
        else: cnt += 1
    return prow, pcol

def getBitmap(xsb, item):
    pbase   = 1 << prow*pcol
    bitmap = pbase
    idx    = 0
    line   = 0
    for c in xsb:
        if(c in charmap[item]):
            bitmap |= 1 << idx
        if(c == '\n'):
            line += 1
            idx = line*pcol
        else: idx += 1
    return bitmap^pbase

def setFrame():
    global frame

    bitmap = 0
    for i in range(pcol):
        bitmap |= 1<<i
    for i in range(prow-2):
        bitmap |= bitmap<<pcol
    frame['u'] = bitmap
    frame['d'] = bitmap<<pcol

    bitmap = 1<<(pcol-1)
    for i in range(prow-1):
        bitmap |= bitmap<<pcol
    frame['l'] = pbase^bitmap
    frame['r'] = (pbase^bitmap)<<1

def setHorizontalDeadZone():
    startpos = 1
    currpos  = 1
    for _ in range(prow-1):
        while not currpos&pspace:
            currpos = getBit(currpos, 'r')
            if not currpos: break


def setDeadzone():
    setHorizontalDeadZone()

def setPuzzle(xsb):
    global prow, pcol, pwall, pgoal, pbox, pman, pbase, pspace

    prow, pcol = getSize(xsb)
    pwall      = getBitmap(xsb, "pwall")
    pgoal      = getBitmap(xsb, "pgoal")
    pbox       = getBitmap(xsb, "pbox")
    pman       = getBitmap(xsb, "pman")

    pbase = 0
    for i in range(pcol):   pbase |= 1<<i
    for i in range(prow-1): pbase |= pbase<<pcol
    pspace = getReach(pman, 0, 0)
    setFrame()

    setDeadzone()

def show(bitmap):
    bitmap = "{0:b}".format(bitmap)
    idx = 0;
    for b in reversed(bitmap):
        print(b, end="")
        if(not (idx+1)%pcol): print()
        idx += 1
    print('\n')

def getBit(pos, drect):
    if(drect == 'u'):
        pos = (pos>>pcol)
    elif(drect == 'd'):
        pos = (pos<<pcol)
    elif(drect == 'l'):
        pos = (pos>>1)
    elif(drect == 'r'):
        pos = (pos<<1)
    return pos&frame[drect]

def getReach(man, box, reach): #FIX
    reach |= man
    space = pspace^box^reach

    for d in dirlist:
        tmp = getBit(man, d)
        if(tmp&space): reach = getReach(tmp, box, reach)

    return reach

def getPushBit(reach, box, drect):
    space  = pspace^box
    pshbit = 0

    pshbit = getBit(box, drect)&space
    box    = box&getBit(pshbit, dirrevr[drect])
    pshbit = space&getBit(box, dirrevr[drect])

    return reach&pshbit

def push(reach, box, drect):
    pshbit = getPushBit(reach, box, drect)
    pos    = []
    idx    = 0

    global reach_time

    while(pshbit):
        beg  = 1<<idx
        idx += 1
        if(beg&pshbit):
            end     = getBit(beg, drect)
            new_box = (box^end)|getBit(end, drect)
            reach   = getReach(end, new_box, 0)
            pos.append({'reach':reach, 'box':new_box, 'move':(beg, end)})
            pshbit ^= beg

    return pos

def findPath(man, node, box):
    que    = queue.Queue()
    vis    = {man}
    path   = []
    pshdir = ''

    que.put([man, []])

    while not que.empty():
        pos = que.get()
        #print(pos, node, end='\n\n\n')
        if(pos[0] == node[0]):
            path = copy.deepcopy(pos[1])
        for d in dirlist:
            man = getBit(pos[0], d)&(pspace^box)
            if(man not in vis):
                vis.add(man)
                p = copy.deepcopy(pos[1])
                p.append(d)
                que.put([man, p])

    for d in dirlist:
        if(getBit(node[0], d) == node[1]):
            pshdir = d
            break

    box = (box^node[1])|getBit(node[1], pshdir)
    path.append(pshdir.upper())

    return path, box

def getMove(nodelist):
    movelist = []
    man      = pman
    box      = pbox

    for node in nodelist:
        moves, box = findPath(man, node, box)
        man        = node[1]
        for move in moves:
            movelist.append(move)

    return  movelist

def solve():
    que   = queue.Queue()
    man   = pman
    box   = pbox
    reach = getReach(man, box, 0)
    vis   = {(reach, box)}
    que.put({'reach':reach, 'box':box, 'move':[]})

    cnt = 0

    while not que.empty():
        pos = que.get()
        #beg = datetime.datetime.now()
        #print(len(vis))

        if pos['box'] == pgoal:
            #print(rt)
            return {'move':getMove(pos['move']), 'cnt':cnt, 'vis':len(vis), 'que':que.qsize()}
        for d in dirlist:
            new_pos = push(pos['reach'], pos['box'], d)
            for np in new_pos:
                if (np['reach'], np['box']) not in vis:
                    vis.add((np['reach'], np['box']))
                    tmp = copy.deepcopy(np)
                    tmp['move'] = copy.deepcopy(pos['move'])
                    tmp['move'].append(np['move'])
                    que.put(tmp)
        #end = datetime.datetime.now()
        #rt += end-beg
        cnt += 1

def main():
    idx = 0
    for xsb in getXSB()[7:8]:
        beg = datetime.datetime.now().replace(microsecond=0)
        setPuzzle(xsb)
        solution = solve()
        end = datetime.datetime.now().replace(microsecond=0)
        print("Puzzle   :", idx)
        print("Time     :", end-beg)
        print("Iteration:", solution['cnt'])
        print("Visited  :", solution['vis'])
        print("Open     :", solution['que'])
        print("Move     :", len(solution['move']), "\n\n")
        idx += 1

if __name__ == '__main__':
    main()
