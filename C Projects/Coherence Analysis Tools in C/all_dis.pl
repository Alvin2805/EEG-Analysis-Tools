#!/usr/local/bin/perl

if( $#ARGV == -1 ){
    printf "./all_dis.pl  <filename(-*.eeg_emg)> <eeg_1st_ch> <eeg_2nd_ch:eeg_start> <eeg_2nd_ch:eeg_end> <time>\n";
    exit;
    }


$FILE = $ARGV[0];
$CH1  = $ARGV[1];
$CH2  = $ARGV[2];
$CH3  = $ARGV[3];
$TIME = $ARGV[4];

system "./csd_all-eeg-electrode legendre_m.pml legendre_m-1.pml ${FILE} ${TIME}";

