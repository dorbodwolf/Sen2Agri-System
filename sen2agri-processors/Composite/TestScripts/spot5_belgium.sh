#USER modif
#add directories where SPOT products are to be found
inputXML="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150410_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150410_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150415_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150415_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150420_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150420_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150505_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150505_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150515_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150515_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150520_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150520_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150604_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150604_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150614_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150614_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150704_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150704_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150709_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150709_N2A_BelgiumD0000B0000.xml "
inputXML+="/mnt/Sen2Agri_DataSets/L2A/Spot5-T5/Belgium/SPOT5_HRG2_XS_20150729_N2A_BelgiumD0000B0000/SPOT5_HRG2_XS_20150729_N2A_BelgiumD0000B0000.xml "
L3A_DATE="20150729"
HALF_SYNTHESIS="50"
BANDS_MAPPING="bands_mapping_spot.txt"
#end of USER modif

if [ $# -lt 4 ]
then
  echo "Usage: $0 <otb directory application> <resolution> <out folder name> <bands mapping file> [scattering coefficient file - S2 case only]"
  echo "The file with input xmls should be given. The resolution for which the computations will be performed should be given. The output directory should be given" 1>&2  
  exit
fi

SCAT_COEF=""
if [ $# == 5 ] ; then    
    ./run_composite.sh "$1" "$inputxml " "$2" "$3" "$L3A_DATE" "$HALF_SYNTHESIS" "$4" "$5"
else
    ./run_composite.sh "$1" "$inputxml " "$2" "$3" "$L3A_DATE" "$HALF_SYNTHESIS" "$4"
fi

