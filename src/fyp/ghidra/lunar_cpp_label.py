#TODO write a description for this script
#@author 
#@category _NEW_
#@keybinding 
#@menupath 
#@toolbar 

# We know that lua cpp class registration functions call the following:
# createtable
# newmetatable
# pushcclosure
# pushlightuserdata
# pushuserdata
# We know that calls to pushstring include "__index"

# So we create lists of xrefs to all and search for entries in all
from ghidra.app.decompiler import DecompInterface
from ghidra.program.model.data import PointerDataType
from ghidra.program.model.symbol import SourceType

xrefs = {}
func_names = [
    "lua_createtable",
    "luaL_newmetatable",
    "lua_pushcclosure",
    "lua_pushlightuserdata",
    "lua_newuserdata" ]


def add_xrefs(function_name, xrefs):
    addr = toAddr(function_name)
    if addr == None:
        print("failed to find {}".format(function_name))
        return
    xrefs[function_name] = currentProgram.referenceManager.getReferencesTo(addr)

for f in func_names:
    print(f)
    add_xrefs(f, xrefs)

parents = {} # Stores functions that contain the xrefs to other functions
fm = currentProgram.getFunctionManager()
rm = currentProgram.getReferenceManager()
af = currentProgram.getAddressFactory()
st = currentProgram.getSymbolTable()
di = DecompInterface()
di.openProgram(currentProgram)

dt_addr = PointerDataType.dataType
print(dt_addr)

def add_parents(function_name, xrefs, parents):

    parents[function_name] = []
    for x in xrefs[function_name]:
        parent_addr = fm.getFunctionContaining(x.getFromAddress())
        if parent_addr == None:
            print("Couldn't find parent to {} in {}".format(x, function_name))
            continue
        parents[function_name].append(parent_addr)

for f in func_names:
    add_parents(f, xrefs, parents)

def is_in_all_lists(thing, lists):
    for l in lists:
        if thing not in l:
            return False
    return True

lists_to_search = []
for f in func_names:
    lists_to_search.append(parents[f])

lua_cpp_reg_funcs = []
for f in func_names:
    for p in parents[f]:
        if is_in_all_lists(p, lists_to_search):
            lua_cpp_reg_funcs.append(p)

print("="*50)
print("Lua CPP class registration functions:")

lua_cpp_reg_funcs = list(set(lua_cpp_reg_funcs))
#for f in lua_cpp_reg_funcs:
#    print(f.getBody())

# ok now we visit each candidate function
# then find all calls to lua_setmetatable
# after each call, the next instruction moves start of table into rsi
def get_first_call_to_function(function, function_name):
    for bodies in function.getBody():
        for address in bodies:
            refs = rm.getReferencesFrom(address)
            for ref in refs:
                if ref.getReferenceType().isCall():
                    called_address = ref.getToAddress()
                    called_function = fm.getFunctionAt(called_address)
                    if called_function:
                        if called_function.getName() == function_name:
                            # ok rsi set above this ref address. Disassemble upwards to find?
                            #print("Call: {}: {}".format(address, called_function.getName()))
                            return called_function
   
def read_str(address, max_length = 50):
    byte_list = []
    for i in range(max_length):
        byte = getByte(address.add(i))
        if byte == 0:
            break
        byte_list.append(byte)
    return ''.join(chr(b) for b in byte_list)

def deref_ptr_ptr_string(address):
    # deref a pointer to a pointer to a string
    ptr_to_str = getDataAt(address).getValue()
    rv = read_str(ptr_to_str)
    return str(rv)

def get_string_parameter(function, function_name, input_num, ptr_ptr = True):

    decompiled = di.decompileFunction(function, 30, None)

    if decompiled is None:
        print("failed to decompile")

    hf = decompiled.getHighFunction()

    rv = []
    second_last_copy = None
    last_copy = None

    for op in hf.getPcodeOps():
        #print('orig op: ' + str(op))
        opcode = op.getOpcode()
        #print('orig opcode: ' + str(opcode))

        #print(op)
       # if opcode == op.COPY:
       #     print('recording copy op input...')
           # print(op)
           # print(op.getInputs())
       #     second_last_copy = last_copy
       #     last_copy = op.getInput(0).getAddress()
        if opcode == op.CALL:

            #print(opcode)
            input_address = op.getInput(0).getAddress()

            #print('input_address is: {}'.format(input_address))
            if input_address is None:
                continue
                #input_address = opcode.getOutput() # This is probably wrong
            
            called_function = fm.getFunctionAt(input_address)
            #print(input_address)
            if called_function is None:
                print('no called function')
                continue


            if called_function.getName() == function_name:

                #print(called_function.getName())
                #print(op.getInputs())
                input_var = op.getInput(input_num)
                if input_var.isAddress():
                    s = deref_ptr_ptr_string(input_var.getAddress())
                    rv.append(s)
                elif input_var.isUnique():
                    d = input_var.getDef();
                    if d.getInput(0).isUnique(): # Gross hack, unused hopefully
                        d = d.getInput(0)
                        d = d.getDef()
                        input_address = d.getInput(0).getAddress()
                    else:
                        input_address = d.getInput(0).getAddress()
                    
                    if input_address.isConstantAddress():
                        input_address = af.getAddress(str(input_address)[6:]) # gross hack, not sure how this should be done
                    s = read_str(input_address)
                    rv.append(s)
                    #uniq_inputs[0].getAddress();
                    #    Data d = currentProgram.getListing.getDataAt(a);
                    #print('resolving call/jmp issue...')
                    # ghidra bug? when last function is "called" with a jmp op.getInput returns a "unique" instead of a "ram" tuple
                    #print(second_last_copy)
#                   # s = deref_ptr_ptr_string(last_copy)
                    #s = read_str(second_last_copy)
                    #rv.append(s)
                    #for subop in bb.getIterator():
                    #    print(opcode)
                 #   print(data2)
    return rv


def get_func_table(function):

    decompiled = di.decompileFunction(function, 30, None)

    if decompiled is None:
        print("failed to decompile")

    hf = decompiled.getHighFunction()

    watch_next = False

    for op in hf.getPcodeOps():
        #print('orig op: ' + str(op))
        opcode = op.getOpcode()
        #print('orig opcode: ' + str(opcode))

        #print(op)
       # if opcode == op.COPY:
       #     print('recording copy op input...')
           # print(op)
           # print(op.getInputs())
       #     second_last_copy = last_copy
       #     last_copy = op.getInput(0).getAddress()
        if opcode == op.CALL:

            #print(opcode)
            input_address = op.getInput(0).getAddress()

            #print('input_address is: {}'.format(input_address))
            if input_address is None:
                continue
                #input_address = opcode.getOutput() # This is probably wrong
            
            called_function = fm.getFunctionAt(input_address)
            #print(input_address)
            if called_function is None:
                continue


            if called_function.getName() == "lua_setmetatable":
                watch_next = True

            elif called_function.getName() == "lua_pushstring" and watch_next == True:
                input_var = op.getInput(2)
                if input_var.isAddress():
                    s = deref_ptr_ptr_string(input_var.getAddress())
                    rv.append(s)
                elif input_var.isUnique():
                    d = input_var.getDef();
                    if d.getInput(0).isUnique(): # Gross hack, unused hopefully
                        d = d.getInput(0)
                        d = d.getDef()
                        input_address = d.getInput(0).getAddress()
                    else:
                        input_address = d.getInput(0).getAddress()
                    
                    if input_address.isConstantAddress():
                        input_address = af.getAddress(str(input_address)[6:]) # gross hack, not sure how this should be done
                    
                    return(input_address)

def clear_data_for_addr(address):
    listing = currentProgram.getListing()
    code_unit = listing.getCodeUnitAt(address)
    if code_unit:
        listing.clearCodeUnits(address, address.add(dt_addr.getLength()), False)

def walk_table(start_address):
    def is_empty(address):
        
        clear_data_for_addr(address)
        clear_data_for_addr(address.add(8))
        clear_data_for_addr(address.add(16))
        clear_data_for_addr(address.add(24))

        #listing.clearCodeUnits(addr, addr.add(pointer_data_type.getLength()), False)
        createData(address, dt_addr)
        createData(address.add(8), dt_addr)
        createData(address.add(16), dt_addr)
        createData(address.add(24), dt_addr)


        data = getDataAt(address)
        if data is None:
           data = getDataAt(address)

        value = getDataAt(address).getValue()
        if value.toString() == "0x0" or value.toString() == "00000000":
            return True
        return False

    def read_block(address):
        if is_empty(address) == True:
            return None
        return {
            "name":  read_str(getDataAt(address).getValue()),
            "func":  getDataAt(address.add(8)),
            "unk1":  getDataAt(address.add(16)),
            "unk2":  getDataAt(address.add(24))
        }
    
    address = start_address

    rv = []
    while True:
        block = read_block(address)
        if block is None:
            break

        address = address.add(8*4)
        rv.append(block)
    return rv

for f in lua_cpp_reg_funcs:
  #  print(f)
  #  print("luaL_newmetatable")
  #  print(f)
    class_name = get_string_parameter(f, "luaL_newmetatable", 2)[0]
    class_lua_name = get_string_parameter(f, "lua_setfield", 3)[-1]
    class_func_table = get_func_table(f)
    print("{} -> {} @ {}".format(class_lua_name, class_name, class_func_table))
    
    func_table = walk_table(class_func_table)

    for t in func_table:
        print("{}:{} -> {}".format(class_lua_name,t["name"],t["func"]))
        func_name = "{}:{}".format(class_lua_name, t["name"])
        print(t["func"])
        if t["func"] is None:
            print("what?")
            continue

        symbol = st.getPrimarySymbol(t["func"].getAddress())
        print(symbol)
        if symbol:
            # Rename the existing symbol
            symbol.setName('lua:' + func_name, SourceType.USER_DEFINED)
        else:
            # Create a new symbol if none exists
            addr = af.getAddress(t["func"].toString()[5:])
            #print(addr)
            st.createLabel(addr, 'lua:' + func_name, SourceType.USER_DEFINED)

 #   print("class name:" + class_name)
#    print("class lua name:" + class_lua_name)

  #  print("lua_setmetatable")
   # func_lua_setmetatable= get_first_call_to_function(f, "lua_setmetatable")








#print(parents)



#TODO Add User Code Here

