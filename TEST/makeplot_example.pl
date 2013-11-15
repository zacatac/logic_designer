#!/usr/bin/perl -w

require 5.001;
#require "./cgi-lib.pl";

my $RES_ROOT="/home/isambert/KINEFOLDPK/SOURCES/TEST/";
my %input;
my $field;

use Text::ParseWords;


MAIN:
{
	my $ligne;
	my $time;
	my $seed;
	#my $nbtrace=0;
	my $nbtrace=1;
	my $compteur;
	my $continuer=1;
	my @legende;
	my $base= $ARGV[0];
#	printf("$base\n");
	

	open(FICH, "/home/isambert/KINEFOLDPK/SOURCES/TEST/$base.e") || die("Error: cannot open the file.");
	
	#push @legende, "Structure";
	push @legende, "Lowest Free Energy";


	while(($continuer==1) && defined ($ligne=<FICH>))
	{
		#if($ligne =~ /^T.*/g)
		if($ligne =~ /^[TtFf].*/g)
		{	
			chomp($ligne);
			$nbtrace ++;
			push @legende, $ligne;
		}
		else
		{
			$continuer=0;
		}
	}
	$time=$ligne;	
	$ligne=<FICH>;				
	$seed=$ligne;	
	$ligne=<FICH>;				
	
	if( $nbtrace!=0)
	{
		my $axecolor="0 0 0";
		my @linecolors=("0 0 0", "1 0 0", "0 1 0", "0 0 1", "0.6 0.6 0", "1 0 1", "0 1 1", "0.5 0.5 0.5", "0.5 0 0.5", "0 0.5 0.5");	
		my @data;
		my $nbbases;
		my $seqname;
		

	
		my $h=450;
		my $w=600;
		my $rmargin=20;	# right margin
		my $tmargin=10;	# top margin
		my $xdecal=40;	# left margin
		#my $ydecal=60;	# bottom margin
		my $ydecal=45;	# bottom margin
		
	
		open(DAT, "</home/isambert/KINEFOLDPK/SOURCES/TEST/$base.dat") || die("Error: cannot open the .dat file.");
		$seqname = <DAT>;
		$lignedat = <DAT>;
		
		
		$seqname =~ s/<//;
		printf "SEQNAME=$seqname";
		$nbbases = length($lignedat)-1;		# -1 because of the \n at the end of the line
		printf "NBbases : $nbbases\n";
	
		# creation of the .eps file		
		open(EPS, ">/home/isambert/KINEFOLDPK/SOURCES/TEST/$base"."_trace.eps") || die("Error: cannot create the eps file.");
		
		
		printf(EPS '%!PS-Adobe-2.0 EPSF-2.0'."\n");
		printf(EPS '%'.'%'.'%'.'Creator: KineFold Server'."\n");
		printf(EPS '%'.'%'.'%'.'%'.'DocumentPaperSizes: Letter'."\n");
		printf(EPS '%'.'%'.'%'.'%'.'Orientation: Landscape'."\n");
		printf(EPS '%'.'%'.'%'.'BoundingBox:'." 0 0 $w $h\n");
		printf(EPS '%'.'%'.'%');
		printf(EPS 'EndComments'."\n");
		printf EPS <<END;

gsave

/Helvetica findfont
12 scalefont
setfont



1 setlinewidth
$axecolor setrgbcolor
$xdecal $ydecal moveto
$xdecal 425 lineto stroke


END
		# displaying the energy on the x axis
		
		# my $Yorigin = $ydecal+($h-$ydecal-$tmargin)/2/1.08;
		my $Yorigin = $ydecal-1+380/2/1.08;
	
		printf EPS ($xdecal +4)." ".($ydecal+380/1.08)." moveto\n";
		printf EPS ($xdecal -4)." ".($ydecal+380/1.08)." lineto stroke\n";
		#printf EPS ($xdecal -26)." ".($ydecal+380/1.08)." moveto\n";
		printf EPS ($xdecal -39)." ".($ydecal+380/1.08+2)." moveto\n";
		printf EPS "(100 %) show\n";
		
#		printf EPS "$xdecal ".($ydecal)." moveto\n";
#		printf EPS ($xdecal -2)." ".($ydecal)." lineto stroke\n";
#		printf EPS ($xdecal -16)." ".($ydecal)." moveto\n";
#		printf EPS "(-1) show\n";

		printf EPS ($xdecal +4)." ".($ydecal+380/2/1.08)." moveto\n";
		printf EPS ($xdecal -4)." ".($ydecal+380/2/1.08)." lineto stroke\n";
		#printf EPS ($xdecal -14)." ".($ydecal+380/2/1.08)." moveto\n";
		printf EPS ($xdecal -27)." ".($ydecal+380/2/1.08+2)." moveto\n";
		printf EPS "(0 %) show\n";
		
#		printf EPS "$xdecal ".($ydecal+380/4/1.08)." moveto\n";
#		printf EPS ($xdecal -2)." ".($ydecal+380/4/1.08)." lineto stroke\n";
#		printf EPS ($xdecal -16)." ".($ydecal+380/4/1.08)." moveto\n";
#		printf EPS "(-0.5) show\n";
		
		printf EPS ($xdecal +4)." ".($ydecal+380/4*3/1.08)." moveto\n";
		printf EPS ($xdecal -4)." ".($ydecal+380/4*3/1.08)." lineto stroke\n";
		#printf EPS ($xdecal -18)." ".($ydecal+380/4*3/1.08)." moveto\n";
		printf EPS ($xdecal -33)." ".($ydecal+380/4*3/1.08+2)." moveto\n";
		printf EPS "(50 %) show\n";
		
		# displaying legends
		#printf EPS ($xdecal -14)." 42 moveto\n";
		#printf EPS ($xdecal -24)." 42 moveto\n";
		#printf EPS ($xdecal +10)." 52 moveto\n";
		printf EPS ($xdecal +10)." ".($ydecal-8)." moveto\n";
		printf EPS "(kcal/mol) show\n";
		#printf EPS ($xdecal -24)." 442 moveto\n";
		#printf EPS ($xdecal +10)." ".($h-14)." moveto\n";
		#printf EPS ($xdecal +10)." 432 moveto\n";
		#printf EPS ($xdecal +10)." ".($ydecal+372)." moveto\n";
		printf EPS ($xdecal +10)." ".($h+$ydecal-74)." moveto\n";
		printf EPS "(Helix Traces) show\n";
		
		#printf EPS ($w-$rmargin-10)." ".($Yorigin -16)." moveto\n";
		printf EPS ($w-$rmargin-60)." ".($Yorigin -30)." moveto\n";
		printf EPS "(Time (sec)) show\n";
		
	
		
		
		while(defined $ligne)
		{		
			@new = quotewords(" ", 0, $ligne);
			push @data, [@new];
			$ligne=<FICH>;				
		}
		
		
		# loading the file is finished ... now let's trace the lines 

#		for($i=0; $i<= ($#data); $i ++)
#		{
#			for($j=0; $j< ($nbtrace + 2); $j ++)
#			{
#				print "$data[$i][$j] ";
#			}
#			print ("\n");
#		}
		
		#my $xcoeff  = ($data[$#data][0]) / ($w-$xdecal-$rmargin);
		#my $xcoeff  = log(1+$data[$#data][0]) / ($w-$xdecal-$rmargin);
		my $xcoeff  = log($data[$#data][0]/$data[0][0]) / ($w-$xdecal-$rmargin);
		my $xcoeff1 = 2*($time) / ($w-$xdecal-$rmargin);
		if(($data[$#data][0]/2)<$time) 
		{
		    $xcoeff1 = ($data[$#data][0]) / ($w-$xdecal-$rmargin);
		}
		#my $xcoeff2 = 2*log(1+$data[$#data][0]-$time) / ($w-$xdecal-$rmargin);
		my $xcoeff2=1;
		my $ff;
		if($time>0.0)
		{
		    $ff=1.2*$time;
		    $xcoeff2 = log($data[$#data][0]/$ff) / ($w-$xdecal-$rmargin-($ff / $xcoeff1));
		    #$xcoeff2 = 2*log($data[$#data][0]/$ff) / ($w-$xdecal-$rmargin-($ff / $xcoeff1));
		    #$xcoeff2 = 2*log($data[$#data][0]/$time) / ($w-$xdecal-$rmargin);
		    #$xcoeff2 = 2*log($data[$#data][0]/(2*$time)) / ($w-$xdecal-$rmargin-(2*$time / $xcoeff1));
		}
		#my $xcoeff3  = $data[$#data][0] / ($w-$xdecal-$rmargin);
		
		# transorming the data to obtain the TOTAL energy of the structure :
		# we must mulptiply the structure energy by the number of bases
		
		for($i=0; $i<=($#data); $i++)
		{
			$data[$i][1] = $data[$i][1] * $nbbases;
		}
		
		my $maxKT = ($data[$#data][1]);
		my $maxKTg;
		
		# determining the max value of the energy "echelle"
		if( $maxKT > -10 )
		{
			$maxKTg = -10;
		}
		elsif( $maxKT > -25 )
		{
			$maxKTg = -25;
		}
		elsif( $maxKT > -50 )
		{
			$maxKTg = -50;
		}
		elsif( $maxKT > -75 )
		{
			$maxKTg = -75;
		}
		elsif( $maxKT > -100 )
		{
			$maxKTg = -100;
		}
		else
		{
			$continuer=1;
			for($i=-150; $continuer==1; $i-=50)
			{
				if( $maxKT > $i )
				{
					$maxKTg = $i;
					$continuer = 0;
				}
			}
		}
		
		printf "maxKT=$maxKT maxKTg=$maxKTg\n";
		
		# diplaying value on the Y axis for the structure's energy
		
		my $ycoeff = -(380/2/1.08) / $maxKTg;
		
		print "ycoeff=$ycoeff\n";
		
		for($i=1; $i<6; $i++)
		{
			printf EPS ($xdecal +4)." ".($Yorigin + ($maxKTg/5*$i*$ycoeff))." moveto\n";
			printf EPS ($xdecal -4)." ".($Yorigin + ($maxKTg/5*$i*$ycoeff))." lineto stroke\n";
			#printf EPS ($xdecal -24)." ".($Yorigin + ($maxKTg/5*$i*$ycoeff))." moveto\n";
			printf EPS ($xdecal -28)." ".($Yorigin + ($maxKTg/5*$i*$ycoeff))." moveto\n";
			printf EPS "(".($maxKTg/5*$i).") show\n";
		}
		
		
		# displaying the time axis
		
		print EPS "$linecolors[0] setrgbcolor\n";
		print EPS ("$xdecal ".($Yorigin)." moveto\n");
		print EPS (($w-$rmargin + 4)." ".($Yorigin)." lineto stroke\n");
		
		#for($i=0; $i< 560 ; $i+=40)
		#{
		#	printf EPS ($xdecal + $i)." ".($Yorigin )." moveto\n";
		#	printf EPS ($xdecal + $i)." ".($Yorigin -8)." lineto stroke\n";
		#	
		#	printf EPS ($xdecal + $i +2)." ".($Yorigin-12)." moveto\n";
		#	printf EPS "(". int($i * $xcoeff).") show\n";
		#	
		#}

		my $g;
		if($time >0.0) 
		{
		    #$ff=2*$time;

		    $g=($ff/5.0);

		    $i=0;
		    $j=int($g/10.0);
		    $jg=int($g);
		    $fg=$g/10.0;
		    while($j!=0 || $jg==0)
		    {
			if($fg>1.0)
			{
			    $fg/=10;
			    $i++;
			}
			else
			{
			    $fg*=10;
			    $i--;
			}
			$j=int($fg/10.0);
			$jg=int($fg);
		    }

		    $j=int($fg + 0.5);
		    #$g=$j*(10**($i+1)); change 23/10/03
		    if($j==0)
		    {
			$g=(10**($i+1));
		    }
		    else
		    {
			$g=$j*(10**($i+1));
		    }
		}
		else
		{
		    $ff=$data[0][0];
		}
		my $imin=0;
		$j=int($ff/10.0);
		my $jj=int($ff);
		my $f=$ff/10.0;
		while($j!=0 || $jj==0)
		{
		    if($f>1.0)
		    {
			$f/=10;
			$imin++;
		    }
		    else
		    {
			$f*=10;
			$imin--;
		    }
		    $j=int($f/10.0);
		    $jj=int($f);
		}
		my $imax=0;
		$j=int($data[$#data][0]/10.0);
		$jj=int($data[$#data][0]);
		$f=$data[$#data][0]/10.0;
		while($j!=0 || $jj==0)
		{
		    if($f>1.0)
		    {
			$f/=10;
			$imax++;
		    }
		    else
		    {
			$f*=10;
			$imax--;
		    }
		    $j=int($f/10.0);
		    $jj=int($f);
		}


		if($time>0.0)
		{
		    $jj=1;
		}
		else
		{
		    $jj=0;
		}
		#for($i=$imin-1; $i<=$imax+1, ($data[$#data][0]/2)>$time; $i++)
		for($i=$imin-1; $i<=$imax+1; $i++)
		{
		    for($j=1; $j<=9 ; $j++)
		    {
			$f=$j*(10**($i));
			#if($f>$xdecal && $f<$data[$#data][0])
			if($f>$ff && $f<$data[$#data][0])
			{
			    #$x2 = $xdecal + log(1+$f) / $xcoeff;
			    if($time>0.0)
			    {
				$x2 = $xdecal + $ff / $xcoeff1 + log($f/$ff) / $xcoeff2;
			    }
			    else
			    {
				$x2 = $xdecal + 4 + log($f/$data[0][0]) / $xcoeff;
			    }
			    if($j==1 || ($j==2 && $time>0.0) || $jj==1)
			    {
				$jj=0;
				printf EPS ($x2)." ".($Yorigin )." moveto\n";
				printf EPS ($x2)." ".($Yorigin -8)." lineto stroke\n";
				$namedecal = 7*length($j*(10**($i-3)));
				$x2 -= $namedecal/2;
				if(($j*(10**($i-3)))>(10**(-3)))
				{
				    printf EPS ($x2)." ".($Yorigin-18)." moveto\n";
				    printf EPS "(".($j*(10**($i-3))).") show\n";
				}
			    }
			    else
			    {
				printf EPS ($x2)." ".($Yorigin )." moveto\n";
				printf EPS ($x2)." ".($Yorigin -4)." lineto stroke\n";
			    }
			}
		    }
		}
		if($time>0.0)
		{
		    $f=$g;
		    while($f<($ff-$g) && $f<$data[$#data][0])
		    {
			printf EPS "$linecolors[1] setrgbcolor\n";
			$x2 = $xdecal + $f / $xcoeff1;
			printf EPS ($x2)." ".($Yorigin )." moveto\n";
			printf EPS ($x2)." ".($Yorigin -8)." lineto stroke\n";
			$namedecal = 7*length($f*(10**(-3)));
			$x2 -= $namedecal/2;
			printf EPS ($x2)." ".($Yorigin-18)." moveto\n";
			printf EPS "(".($f*(10**(-3))).") show\n";
			$f+=$g;
		    }
		}
		
#		for($i=0; $i <400; $i +=10)
#		{
#			print EPS "$xdecal $i moveto\n";
#			print EPS "($i) show\n";
#		}
		
		# drawing the energy of the structure :
	
		#$x1 = $xdecal + $data[0][0] / $xcoeff;
		if($time >0.0)
		{ 
		    $x1 = $xdecal + $data[0][0] / $xcoeff1;
		}
		else
		{
		    #$x1 = $xdecal + $data[0][0] / $xcoeff;
		    #$x1 = $xdecal + log(1+$data[0][0]) / $xcoeff;
		    $x1 = $xdecal+4;
		}
		$y1 = $data[0][1] *$ycoeff + $Yorigin;
		print EPS "$linecolors[1] setrgbcolor\n";
		#print EPS "$xdecal $y1 moveto\n";
		print EPS "$x1 $Yorigin moveto\n";
		print EPS "$x1 $y1 lineto stroke\n";
		
		for($i=1; $i<= ($#data); $i++)
		{
			#$x2 = $xdecal + $data[$i][0] / $xcoeff;

		        if($time >0.0)
			{ 
			    #if($data[$i][0]<$time || (($data[$#data][0]/2)<$time))
			    if($data[$i][0]<$ff || (($data[$#data][0]/2)<$time))
			    { 
				$x2 = $xdecal + $data[$i][0] / $xcoeff1;
			    }
			    else
			    {
				#$x2 = $xdecal + $time / $xcoeff1 + log(1+$data[$i][0]-$time) / $xcoeff2;
				#$x2 = $xdecal + $time / $xcoeff1 + log($data[$i][0]/$time) / $xcoeff2;
				$x2 = $xdecal + $ff / $xcoeff1 + log($data[$i][0]/$ff) / $xcoeff2;
			    }
			}
			else
			{
			    #$x2 = $xdecal + $data[$i][0] / $xcoeff;
			    #$x2 = $xdecal + log(1+$data[$i][0]) / $xcoeff;
			    $x2 = $xdecal + 4 + log($data[$i][0]/$data[0][0]) / $xcoeff;
		        }
			$y2 = $data[$i][1] *$ycoeff + $Yorigin;
			print EPS "$x1 $y1 moveto\n";
			print EPS "$x2 $y2 lineto stroke\n";
			
			$x1 = $x2;
			$y1 = $y2;	
		}
		
		# drawing the energy of helixes :
		
		for($i=2; $i< ($nbtrace+1); $i ++)
		{
			$j=0;
		
			#$x1 = $xdecal + $data[$j][0] / $xcoeff;
			if($time >0.0)
			{ 
			    $x1 = $xdecal + $data[$j][0] / $xcoeff1;
			}
			else
			{
			    #$x1 = $xdecal + $data[$j][0] / $xcoeff;
			    #$x1 = $xdecal + log(1+$data[$j][0]) / $xcoeff;
			    #$x1 = $xdecal + log($data[$j][0]/$data[0][0]) / $xcoeff;
			    $x1 = $xdecal + 4;
			}
			$y1 = $ydecal + $data[$j][$i] *380/2/1.08 +380/2/1.08;
			print EPS "$linecolors[$i] setrgbcolor\n";
			#print EPS "$xdecal $y1 moveto\n";
			print EPS "$x1 $Yorigin moveto\n";
			print EPS "$x1 $y1 lineto stroke\n";
			
			#for($j=0; $j<= ($#data); $j++)
			for($j=1; $j<= ($#data); $j++)
			{
				
				#$x2 = $xdecal + $data[$j][0] / $xcoeff;
			        if($time >0.0)
				{ 
				    #if($data[$j][0]<$time || (($data[$#data][0]/2)<$time))
				    if($data[$j][0]<$ff || (($data[$#data][0]/2)<$time))
				    { 
					$x2 = $xdecal + $data[$j][0] / $xcoeff1;
				    }
				    else
				    {
					#$x2 = $xdecal + $time / $xcoeff1 + log(1+$data[$j][0]-$time) / $xcoeff2;
					#$x2 = $xdecal + $time / $xcoeff1 + log($data[$j][0]/$time) / $xcoeff2;
					$x2 = $xdecal + $ff / $xcoeff1 + log($data[$j][0]/$ff) / $xcoeff2;
				    }
				}
				else
				{
				    #$x2 = $xdecal + $data[$j][0] / $xcoeff;
				    #$x2 = $xdecal + log(1+$data[$j][0]) / $xcoeff;
				    $x2 = $xdecal + 4 + log($data[$j][0]/$data[0][0]) / $xcoeff;
				}
				$y2 = $ydecal + $data[$j][$i] *380/2/1.08 +380/2/1.08;
				print EPS "$x1 $y1 moveto\n";
				print EPS "$x2 $y2 lineto stroke\n";
				
				$x1 = $x2;
				$y1 = $y2;

			}
		}
		
		# displaying the legend
#$xdecal += 30;		
		print EPS <<END;
0 0 0 setrgbcolor
$xdecal 30 moveto
580 30 lineto stroke
580 30 moveto
580 14 lineto stroke
580 14 moveto
$xdecal 14 lineto stroke
$xdecal 14 moveto
$xdecal 30 lineto stroke
/Helvetica findfont
12 scalefont
setfont
END
$xdecal -= 30;		

		$y1 = 18;
		#$y1 = 28;
		#$x1 = $xdecal+20;
		$x1 = $xdecal+40;
			print EPS "$linecolors[1] setrgbcolor\n";
			print EPS "$x1 $y1 moveto\n";
			print EPS "($legende[0]) show\n";
		        $x1 += 140;
		#for($i=0; $i<($nbtrace); $i ++)
		for($i=1; $i<($nbtrace); $i ++)
		{
			print EPS "$linecolors[$i+1] setrgbcolor\n";
			print EPS "$x1 $y1 moveto\n";
			print EPS "($legende[$i]) show\n";
			
			$y1 = 18;
			#$y1 = 28;
			$x1 += 80;
			#if($i==0) $x1 += 20;

			#$y1 -= 16;
			#if($y1<0)
			#{
			#	$y1 = 38;
			#	$x1 += 80;
			#}
		}
		

		# printing the name of the sequence
		#$namedecal = 8*length($seqname);
		$namedecal = 8*18;
		
		print EPS "$linecolors[0] setrgbcolor\n";
		#print EPS ($w-$namedecal)." ".($h-14)." moveto\n";
		print EPS ($w-$namedecal)." ".($h+$ydecal-74)." moveto\n";
		#print EPS "($seqname) show\n";
		print EPS "(generated by KineFold) show\n";
		
print EPS <<END;
/Helvetica findfont
10 scalefont
setfont
END
		if($time>0.0)
		{
		    #printf EPS ($w-$rmargin-360)." ".($Yorigin -28)." moveto\n";
		    #printf EPS "(End of transcription) show\n";
		    #printf EPS "$linecolors[1] setrgbcolor\n";
		    #printf EPS ($w-$rmargin-360)." ".($Yorigin -32)." moveto\n";
		    #printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -32)." lineto stroke\n";
		    #printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -32)." moveto\n";
		    #printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -4)." lineto stroke\n";
		    #printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -4)." moveto\n";
		    #printf EPS ($xdecal + 26 + $time / $xcoeff1)." ".($Yorigin -14)." lineto stroke\n";
		    #printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -4)." moveto\n";
		    #printf EPS ($xdecal + 34 + $time / $xcoeff1)." ".($Yorigin -14)." lineto stroke\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1 - 100)." ".($Yorigin -34)." moveto\n";
		    printf EPS "(End of transcription) show\n";
		    printf EPS "$linecolors[1] setrgbcolor\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1 - 100)." ".($Yorigin -38)." moveto\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -38)." lineto stroke\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -38)." moveto\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -20)." lineto stroke\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -20)." moveto\n";
		    printf EPS ($xdecal + 26 + $time / $xcoeff1)." ".($Yorigin -30)." lineto stroke\n";
		    printf EPS ($xdecal + 30 + $time / $xcoeff1)." ".($Yorigin -20)." moveto\n";
		    printf EPS ($xdecal + 34 + $time / $xcoeff1)." ".($Yorigin -30)." lineto stroke\n";
		}
print EPS <<END;
/Helvetica findfont
16 scalefont
setfont
END
	        #$namedecal = 8*length($seqname);
	        $namedecal = 8*length($seqname)+8*length($seed)+20;
		$namedecal = ($w-$namedecal)/2;
		print EPS "$linecolors[0] setrgbcolor\n";
		print EPS ($namedecal)." ".($h+$ydecal-67)." moveto\n";
		print EPS "($seqname) show\n";
print EPS <<END;
/Helvetica findfont
12 scalefont
setfont
END
                #$g=$xdecal + $ff / $xcoeff1 + log($data[$#data][0]/$ff) / $xcoeff2;
		$f=$data[$#data][0];
		$namedecal += 8*length($seqname)+15;
		print EPS ($namedecal)." ".($h+$ydecal-67)." moveto\n";
		print EPS "($seed) show\n";
		
		printf EPS "grestore\n";
		close EPS;
		
		system("convert -quality 100 /home/isambert/KINEFOLDPK/SOURCES/TEST/$base"."_trace.eps /home/isambert/KINEFOLDPK/SOURCES/TEST/$base"."_trace.jpg");
	}
}















