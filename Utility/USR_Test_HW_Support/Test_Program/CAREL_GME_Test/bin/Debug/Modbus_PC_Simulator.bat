REM this script is launched automatically by CAREL_GME_Test.exe
REM the com port is set in CAREL_GME_Test_CFG.ini
python.exe Modbus_PC_Simulator.py -p %1
pause