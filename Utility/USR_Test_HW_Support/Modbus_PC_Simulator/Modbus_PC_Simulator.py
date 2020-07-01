# CAREL Industries SpA
#
# Modbus_PC_Simulator.py
#
# Description : 
# this program is a support to the test program for the GME device on production
# line the only functionality is to return 1234 non HR idx 1
#

# --------------------------------------------------------------------------- #
# import the various server implementations
# --------------------------------------------------------------------------- #
import sys, getopt
from pymodbus.server.sync import StartTcpServer
from pymodbus.server.sync import StartTlsServer
from pymodbus.server.sync import StartUdpServer
from pymodbus.server.sync import StartSerialServer

from pymodbus.device import ModbusDeviceIdentification
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSparseDataBlock
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext

from pymodbus.transaction import ModbusRtuFramer, ModbusBinaryFramer

# --------------------------------------------------------------------------- #
# configure the service logging
# --------------------------------------------------------------------------- #
import logging
FORMAT = ('%(asctime)-15s %(threadName)-15s'
          ' %(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
logging.basicConfig(format=FORMAT)
log = logging.getLogger()
log.setLevel(logging.DEBUG)


def run_server(comport):

    store = ModbusSlaveContext(
        di=ModbusSequentialDataBlock(0, [1234]*100),
        co=ModbusSequentialDataBlock(0, [1234]*100),
        hr=ModbusSequentialDataBlock(0, [1234]*100),
        ir=ModbusSequentialDataBlock(0, [1234]*100))

    context = ModbusServerContext(slaves=store, single=True)

    # ----------------------------------------------------------------------- #
    # initialize the server information
    # ----------------------------------------------------------------------- #
    # If you don't set this or any fields, they are defaulted to empty strings.
    # ----------------------------------------------------------------------- #
    identity = ModbusDeviceIdentification()
    identity.VendorName = 'CAREL'
    identity.ProductName = 'Modbus Server'
    identity.ModelName = 'Modbus Server for GME test'
    identity.MajorMinorRevision = '1.0.0'

    # ----------------------------------------------------------------------- #
    # run the server
    # ----------------------------------------------------------------------- #
    # RTU:
    print 'Use comport "', comport
    StartSerialServer(context, framer=ModbusRtuFramer, identity=identity, port=comport, timeout=.005, baudrate=19200, bytesize=8, parity='N', stopbits=1 )


def main(argv):

   try:
      opts, args = getopt.getopt(argv,"hp:",["port="])
   except getopt.GetoptError:
      print 'Modbus_PC_Simulator.py -p <comport> '
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'Modbus_PC_Simulator.py -p <comport> -o <outputfile>'
         sys.exit()
      elif opt in ("-p", "--port"):
         comport = arg

   run_server(comport)



if __name__ == "__main__":
   main(sys.argv[1:])





