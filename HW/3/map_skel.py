###########################
# mapping optimization
###########################

import gurobipy as gp
from gurobipy import GRB

print("=============================================");
print("mapping ILP optimizer");
print("=============================================");

# benchmark information

my_infty = 10000000
# num tasks: 4
num_tasks = 4
# num procs: 4
num_procs = 4
# latency constraint
latency_const = 5

# exec. info
exec_time = [
[1,2,3,4],
[2,1,3,4],
[2,3,1,4],
[2,3,3,1],
]

# edge info
edge = [
[0,1,1,0],
[0,0,0,1],
[0,0,0,1],
[0,0,0,0],
]

# cost info
cost = [1, 2, 3, 4]

# model generation
m = gp.Model("mapping")

# mapping variables m
m_var = [] # m variables
for i in range(num_tasks):
	temp_m_var = []
	mylhs = gp.LinExpr(0.0) 
	for j in range(num_procs):
		temp_m_var.append(m.addVar(vtype=GRB.BINARY, name="m_" +str(i) + "_" + str(j))) # binary variable m_i_j
		mylhs.add(temp_m_var[j])
	m.addConstr(lhs=mylhs, sense=GRB.EQUAL, rhs=1, name="mapping_const_"+str(i))
	m_var.append(temp_m_var)

# starting time variables s
s_var = [] # s variables
for i in range(num_tasks):
	s_var.append(m.addVar(vtype=GRB.INTEGER, name="s_" + str(i))) # integer variable s_i

# execution time variables t
t_var = [] # t variables
for i in range(num_tasks):
	t_var.append(m.addVar(vtype=GRB.INTEGER, name="t_" + str(i))) # integer variable t_i
	myrhs = gp.LinExpr(0.0) 
	for j in range(num_procs):
		myrhs.add(exec_time[i][j]* m_var[i][j])
	m.addConstr(lhs=t_var[i], sense=GRB.EQUAL, rhs=myrhs, name="exec_time_const_"+str(i))

# latency constraints 
for i in range(num_tasks):
	mylhs = gp.LinExpr(0.0) 
	mylhs.add(s_var[i])
	mylhs.add(t_var[i])
	m.addConstr(lhs=mylhs, sense=GRB.LESS_EQUAL, rhs=latency_const, name="latency_const_"+str(i))


# edge constraints
for i in range(num_tasks):
	for j in range(num_tasks):
		if edge[i][j] == 1:
			m.addConstr(s_var[i] + t_var[i] <= s_var[j], name="edge_const_"+str(i)+"_"+str(j))

# meta variables sh, a
sh_var = []
ah_var = []
for i in range(num_tasks):
	temp_sh_var = []
	temp_ah_var = []
	for j in range(num_tasks):
		temp_sh_var.append(m.addVar(vtype=GRB.BINARY, name="sh_" + str(i) + "_" + str(j))) # binary variable sh_i_j
		temp_ah_var.append(m.addVar(vtype=GRB.BINARY, name="ah_" + str(i) + "_" + str(j))) # binary variable ah_i_j
	sh_var.append(temp_sh_var)
	ah_var.append(temp_ah_var)
		
# linearized scheduling constraint 1
for i in range(num_tasks):
	for j in range(num_tasks):
		if i!=j:
			# complete this part
			m.addConstr(s_var[i] + t_var[i] <= s_var[j] + (1 - sh_var[i][j])*my_infty + (1 - ah_var[i][j])*my_infty, name="schedulingConstraint1"+str(i)+"_"+str(j))

# linearized scheduling constraint 2
for i in range(num_tasks):
	for j in range(num_tasks):
		if i!=j:
			# complete this part
			m.addConstr(s_var[j] + t_var[j] <= s_var[i] + (1 - sh_var[i][j])*my_infty + ah_var[i][j]*my_infty, name="schedulingConstraint2"+str(i)+"_"+str(j))


# binary variables used
used_vars = []
for i in range(num_procs):
	used_vars.append(m.addVar(vtype=GRB.BINARY, name="used_"+str(i)))
	myrhs = gp.LinExpr(0.0)
	for j in range(num_tasks):
		m.addConstr(lhs=used_vars[i], sense=GRB.GREATER_EQUAL, rhs=m_var[j][i], name="used_const_"+str(i)+"_"+str(j))


# sh constraints 1
for k in range(num_procs):
        for i in range(num_tasks):
                for j in range(num_tasks):
                        if i!=j:
                                mylhs = gp.LinExpr(0.0)
                                myrhs = gp.LinExpr(0.0)
                                m.addConstr(sh_var[i][j] >= m_var[i][k] + m_var[j][k] -1, name="sh_const_"+str(k)+"_"+str(i)+"_"+str(j))

# sh constraints 2
for k in range(num_procs):
        for l in range(num_procs):
                for i in range(num_tasks):
                        for j in range(num_tasks):
                                if i!=j and k!=l:
                                        mylhs = gp.LinExpr(0.0)
                                        myrhs = gp.LinExpr(0.0)
                                        m.addConstr(sh_var[i][j] <= (1-m_var[i][k])*my_infty + (1-m_var[j][l])*my_infty , name="sh_const_sec_"+str(k)+"_"+str(l)+"_"+str(i)+"_"+str(j))



# objective
myobj = gp.LinExpr(0.0)
for i in range(num_procs):
	myobj.add(used_vars[i] * cost[i])
m.setObjective(myobj, GRB.MINIMIZE)

m.write("formulation.lp")
m.optimize()

print("=============================================")
print("Optimization Results:")
m.printAttr('X')
print("=============================================")
