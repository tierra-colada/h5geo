#ifndef H5SEGY_HEADERS_H
#define H5SEGY_HEADERS_H

#include <string>
#include <vector>

namespace h5geo
{
std::vector<std::string> getRawBinHeaderNames()
{
  return
  {
    {"Job identification number.	JOB	3201	4"},
    {"Line number.  For 3-D poststack data, this will typically contain the in-line number.	LINE	3205	4"},
    {"Reel number.	REEL	3209	4"},
    {"Number of data traces per ensemble.	TRACENUM	3213	2"},
    {"Number of auxiliary traces per ensemble.	AUX	3215	2"},
    {"Sample interval in microseconds (µs).	SAMP_RATE	3217	2"},
    {"Sample interval in microseconds  (µs) of original field recording.	SAMP_FRATE	3219	2"},
    {"Number of samples per data trace.	SAMP_NUM	3221	2"},
    {"Number of samples per data trace for original field recording.	SAMP_FNUM	3223	2"},
    {"Data sample format code.  Mandatory for all data. 1=4-byte IBM float; 2=4-byte, two`s complement integer; 3=2-byte, two`s complement integer; 4=4-byte fixed-point with gain (obsolete); 5=4-byte IEEE float; 6,7=not used; 8=1-byte, two`s complement integer	FORMAT	3225	2"},
    {"CMP fold.	CDP_FOLD	3227	2"},
    {"Trace sorting code. -1=Other (should be explained in user Extended Textual Header); 0=Unknown; 1=As recorded (no sort); 2=CDP; 3=Single fold continuous profile; 4=Horizontally stacked; 5=Common source; 6=Common receiver; 7=Common offset; 8=Common mid-point; 9=Common conversion point	SORT	3229	2"},
    {"Vertical sum code. 1=no sum; 2=two sum; ... N=M-1 sum (M=2 to 32,767)	VERT_SUM	3231	2"},
    {"Sweep frequency at start (Hz).	SWEEP_START	3233	2"},
    {"Sweep frequency at end (Hz).	SWEEP_END	3235	2"},
    {"Sweep length (ms).	SWEEP_LENGTH	3237	2"},
    {"Sweep type code. 1=linear; 2=parabolic; 3=exponential; 4=other	SWEEP_TYPE	3239	2"},
    {"Trace number of sweep channel	SWEEP_CHAN	3241	2"},
    {"Sweep trace taper length in milliseconds at start if tapered	SWEEP_TAPER_START	3243	2"},
    {"Sweep trace taper length in milliseconds at end	SWEEP_TAPER_END	3245	2"},
    {"Taper type. 1=linear; 2=cos^2; 3=other	SWEEP_TAPER_TYPE	3247	2"},
    {"Correlated data traces. 1=no; 2=yes	CORR	3249	2"},
    {"Binary gain recovered. 1=yes; 2=no	BIN_GAIN	3251	2"},
    {"Amplitude recovery method. 1=none; 2=spherical divergence; 3=AGC; 4=other	AMP_REC	3253	2"},
    {"Measurement system. 1=Meters; 2=Feet	LENGTH_SYS	3255	2"},
    {"Impulse signal polarity. 1=Increase in pressure or upward geophone case movement gives negative number on tape; 2=Increase in pressure or upward geophone case movement gives positive number on tape	POLARITY	3257	2"},
    {"Vibratory polarity code: Seismic signal lags pilot signal by: 1=337.5 to 22.5; 2=22.5 to 67.5; 3=67.5 to 112.5; 4=112.5 to 157.5; 5=157.5 to 202.5; 202.5 to 247.5; 7=247.5 to 292.5; 8=292.5 to 337.5	VIB_POL	3259	2"},
    {"SEGY format revision number.	REVISION	3501	2"},
    {"Fixed length trace flag. 0=trace length may vary and the actual length is stored in 115-116 bytes of trace header; 1=all traces have the same length	FIXED_TRLENGTH	3503	2"},
    {"Extended textual header. 0=there are no additional textual header; positive number indicates that there exactly that many Extended Textual Headers	N_EXT_HDRS	3505	2"}
  };
}

std::vector<std::string> getRawTraceHeaderNames()
{
  return
  {
    {"* Trace sequence number within line	SEQWL	1	4"},
    {"Trace sequence number within reel	SEQWR	5	4"},
    {"* FFID - Original field record number	FFID	9	4"},
    {"* Trace number within field record	TRCFLD	13	4"},
    {"SP - Energy source point number	SP	17	4"},
    {"CDP ensemble  number	CDP	21	4"},
    {"Trace  number	TRCNUM	25	4"},
    {"* Trace identification code	TRCID	29	2"},
    {"Number of vertically summed traces	NVST	31	2"},
    {"Number of horizontally stacked traces	NHST	33	2"},
    {"Data use ( 1-production, 2-test )	DU	35	2"},
    {"Distance from source point to receiv grp	DSREG	37	4"},
    {"Receiver group elevation	RGE	41	4"},
    {"Surface elevation at source	SES	45	4"},
    {"Source depth below surface	SDBS	49	4"},
    {"Datum elevation at receiver group	DERG	53	4"},
    {"Datum elevation at source	DES	57	4"},
    {"Water depth at source	WDS	61	4"},
    {"Water depth at group	WGD	65	4"},
    {"Scaler to all elevations & depths	SAED	69	2"},
    {"Scaler to all coordinates	SAC	71	2"},
    {"Source X coordinate	SRCX	73	4"},
    {"Source Y coordinate	SRCY	77	4"},
    {"h5gt::Group  X coordinate	GRPX	81	4"},
    {"h5gt::Group  Y coordinate	GRPY	85	4"},
    {"Coordinate units (1-lenm/ft 2-secarc)	UNITS	89	2"},
    {"Weathering velocity	WVEL	91	2"},
    {"Subweathering velocity	SVEL	93	2"},
    {"Uphole time at source	UTSRC	95	2"},
    {"Uphole time at group	UTGRP	97	2"},
    {"Source static correction	SECSCOR	99	2"},
    {"h5gt::Group  static correction	GRPSCOR	101	2"},
    {"Total static applied	TSA	103	2"},
    {"Lag time A	LAGTA	105	2"},
    {"Lag time B	LAGTB	107	2"},
    {"Delay Recording time	DELRECT	109	2"},
    {"Mute time start	MTSTART	111	2"},
    {"Mute time end	MTEND	113	2"},
    {"* Number of samples in this trace	NSMP	115	2"},
    {"* Sample interval in ms for this trace	SI	117	2"},
    {"Gain type of field instruments	GTFI	119	2"},
    {"Instrument gain	IG	121	2"},
    {"Instrument gain constant	IGC	123	2"},
    {"Correlated (1-yes / 2-no)	CORREL	125	2"},
    {"Sweep frequency at start	SFSTART	127	2"},
    {"Sweep frequency at end	SFEND	129	2"},
    {"Sweep lenth in ms	SLEN	131	2"},
    {"Sweep type 1-lin,2-parabol,2-exp,4-ohter	STYP	133	2"},
    {"Sweep trace taper length at start in ms	SSTRLS	135	2"},
    {"Sweep trace taper length at end   in ms	SSTLE	137	2"},
    {"Taper type 1-lin,2-cos2,3-other	TTYP	139	2"},
    {"Alias filter frequency, if used	AFF	141	2"},
    {"Alias filter slope	AFS	143	2"},
    {"Notch filter frequency, if used	NFF	145	2"},
    {"Notch filter slope	NFS	147	2"},
    {"Low cut frequency,  if used	LOCF	149	2"},
    {"High cut frequency, if used	HOCF	151	2"},
    {"Low cut slope	LOCS	153	2"},
    {"High cut slope	HICS	155	2"},
    {"Year data recorded	YEAR	157	2"},
    {"Day of year	DAY	159	2"},
    {"Hour of day	HOUR	161	2"},
    {"Minute of hour	MINUTE	163	2"},
    {"Second of minute	SCE	165	2"},
    {"Time basis code 1-local,2-GMT,3-other	TMBS	167	2"},
    {"Trace weighting factor	TWF	169	2"},
    {"Geophone group number of roll sw pos 1	GGNSW	171	2"},
    {"Geophone group number of trace # 1	GGN1ST	173	2"},
    {"Geophone group number of last trace	GGNLST	175	2"},
    {"Gap size (total # of groups dropped)	GAPSZ	177	2"},
    {"Overtravel assoc w taper of beg/end line	OAWT	179	2"},
    {"+ CDP X	CDP_X	181	4"},
    {"+ CDP Y	CDP_Y	185	4"},
    {"+ Inline Number	INLINE	189	4"},
    {"+ Clossline Number	XLINE	193	4"},
    {"+ Shot Point Number	SPN	197	4"},
    {"+ Shot Point Scalar	SPS	201	2"},
    {"+ Trace value measurement unit	TVMU	203	2"}
  };
}
}

#endif // H5SEGY_HEADERS_H
