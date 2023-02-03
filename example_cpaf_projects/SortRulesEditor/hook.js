dbgStdOut('ML_DBG:  create_display_sets: series.dicom("SeriesDescription"): ' +  series.dicom("SeriesDescription") );
dbgStdOut('ML_DBG:  create_display_sets: series.childCount(): ' +  series.childCount());
var x = new DisplaySet();
x.setLabel("X");
return [x];
