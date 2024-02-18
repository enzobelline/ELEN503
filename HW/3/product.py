#!/usr/bin/env python3.7

# Copyright 2020, Gurobi Optimization, LLC

# This example formulates and solves the following simple MIP model:
#  maximize
#        x +   y + 2 z
#  subject to
#        x + 2 y + 3 z <= 4
#        x +   y       >= 1
#        x, y, z binary

import gurobipy as gp
from gurobipy import GRB

try:

    # Create a new model
    m = gp.Model("prod")

    # Create variables
    x1 = m.addVar(vtype=GRB.INTEGER, name="x1")
    x2 = m.addVar(vtype=GRB.INTEGER, name="x2")

    # Set objective
    m.setObjective(15 * x1 + 20 * x2, GRB.MAXIMIZE)

    # Add constraint c2: x1 + 2 * x2 <= 20
    m.addConstr(x1  + 2 * x2 <= 20, "c2")

    # Add constraint c2: 2* x1 + x2 <= 20
    m.addConstr(2 * x1  +  x2 <= 20, "c3")

    # Add constraint x1>=0, x2>=0
    m.addConstr(x1 >= 0, "c4_1")
    m.addConstr(x2 >= 0, "c4_2")

    # Optimize model
    m.optimize()

    for v in m.getVars():
        print('%s %g' % (v.varName, v.x))
        print('%s %g' % (v.varName, v.x))

    print('Obj: %g' % m.objVal)

except gp.GurobiError as e:
    print('Error code ' + str(e.errno) + ': ' + str(e))

except AttributeError:
    print('Encountered an attribute error')
