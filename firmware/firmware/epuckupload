#! /usr/bin/perl
use strict;

# e-puck Bootloader for Linux
# Version: 2006-02-22
#
# (c) 2006 SWIS Group, EPFL, Switzerland
# written by Xavier Raemy and Thomas Lochmatter
# http://swis.epfl.ch
# http://www.epfl.ch
#
# This script is free software.
# Please report bugs to one of the authors.

# Create an EpuckBootloadobject
my $ebl=EpuckBootloader->new();

# Parse arguments
my $verbose=0;
while (my $arg=shift) {
	if (($arg eq '-v') || ($arg eq '-verbose') || ($arg eq '--verbose')) {
		$verbose=1;
	} elsif (($arg eq '-h') || ($arg eq '-help') || ($arg eq '--help')) {
		&help();
	} elsif (($arg eq '-bh') || ($arg eq '-bootloaderhelper') || ($arg eq '--bootloaderhelper')) {
		$ebl->openINHX32bootloaderhelper(shift);
	} elsif (($arg eq '-b') || ($arg eq '-bootloader') || ($arg eq '--bootloader')) {
		$ebl->openINHX32bootloader(shift);
	} elsif (($arg eq '-f') || ($arg eq '-file') || ($arg eq '--file')) {
		$ebl->openINHX32(shift);
	} elsif (($arg eq '-j') || ($arg eq '-parallel') || ($arg eq '--parallel')) {
		$ebl->maxparallel(shift);
	} elsif (($arg eq '-n') || ($arg eq '-maxattempts') || ($arg eq '--maxattempts')) {
		$ebl->maxattempts(shift);
	} elsif (($arg eq '-w') || ($arg eq '-waitforreset') || ($arg eq '--waitforreset')) {
		$ebl->waitforreset(shift);
	} elsif (($arg eq '-l') || ($arg eq '-successlight') || ($arg eq '--successlight')) {
		$ebl->successlight(shift);
	} elsif (($arg eq '-s') || ($arg eq '-silent') || ($arg eq '--silent')) {
		$ebl->belltoreset(0);
	} elsif ($arg =~ /^-/) {
		print 'Unrecognized option: ', $arg;
		exit(1);
	} else {
		$arg=$1 if ($arg=~/^\s*(.*)\s*/);
		if ($arg =~ /^\d+$/) {
			my $epuckid=$arg;
			my $device=$epuckid;
			while ($device>200) {$device-=200;}
			$device='/dev/rfcomm'.$device;
			$ebl->adddevice($device, $epuckid);
		} elsif ($arg =~ /^epuck(\d+)$/) {
			my $epuckid=int($1);
			my $device=$epuckid;
			while ($device>200) {$device-=200;}
			$device='/dev/rfcomm'.$device;
			$ebl->adddevice($device, $epuckid);
		} elsif ($arg =~ /^e-puck_(\d+)$/) {
			my $epuckid=int($1);
			my $device=$epuckid;
			while ($device>200) {$device-=200;}
			$device='/dev/rfcomm'.$device;
			$ebl->adddevice($device, $epuckid);
		} else {
			$ebl->adddevice($arg, 0);
		}
	}
}

# Show help if arguments are missing
&help() if (! $ebl->getcountprograms());

# In verbose mode, write the memory and the packets
if ($verbose) {
	$ebl->writefilehex('epuckupload.memory');
	$ebl->log('Dump of memory to program written to epuckupload.memory');
	$ebl->writepackets('epuckupload.packets');
	$ebl->log('Packets written to epuckupload.packets');
}

# Read devices from STDIN if none are given on the command line
if (! $ebl->getcountdevices()) {
	$ebl->log('Looking for epucks on STDIN');
	while (my $line=<>) {
		my @epucks=($line=~/e-puck_(\d+)/ig);
		foreach my $epuckid (@epucks) {
			$epuckid=int($epuckid);
			my $device=$epuckid;
			while ($device>200) {$device-=200;}
			$device='/dev/rfcomm'.$device;
			$ebl->adddevice($device, $epuckid);
		}
		my @epucks=($line=~/epuck(\d+)/ig);
		foreach my $epuckid (@epucks) {
			$epuckid=int($epuckid);
			my $device=$epuckid;
			while ($device>200) {$device-=200;}
			$device='/dev/rfcomm'.$device;
			$ebl->adddevice($device, $epuckid);
		}
	}
}

# Upload
$ebl->send();

# Prints a help text
sub help {
	print 'E-Puck Bootloader', "\n";
	print 'Usage: epuckupload [OPTIONS] -f FILE [DEVICE1 DEVICE2 ...]', "\n";
	print '', "\n";
	print 'Options:', "\n";
	print '-j NUM     Upload to NUM e-pucks in parallel (default: 5)', "\n";
	print '-n NUM     Maximum number of attempts (default: 5)', "\n";
	print '-w SEC     Seconds to wait for the e-puck to reset (default: 30)', "\n";
	print '-l SEC     Seconds to switch on the light on success (default: 2)', "\n";
	print '-bh BOOTLOADERHELPER  Uploads a new bootloader helper', "\n";
	print '-b BOOTLOADER         Uploads a new bootloader', "\n";
	print '-s         Silent mode (don\'t bell to reset)', "\n";
	exit(1);
}

# This class implements the epuck bootloader.
package EpuckBootloader;
use Time::HiRes;

# Creates a new object.
sub new {	# new()
	my $o=shift;
	my $class=(ref($o) || $o);
	my $newo=bless {}, $class;
	$newo->init(@_);
	return $newo;
}

# Initializes the object.
sub init {	# init()
	my $o=shift;
	$o->{'programs'}=[];
	$o->{'devices'}=[];
	$o->{'maxparallel'}=5;
	$o->{'maxattempts'}=5;
	$o->{'waitforreset'}=30;
	$o->{'successlight'}=2;
	$o->{'belltoreset'}=1;
	return;
}

# Returns the number of programs to upload.
sub getcountprograms {
	my $o=shift;
	my $programs=$o->{'programs'};
	return scalar @$programs;
}

# Returns the number of programs to upload.
sub getcountdevices {
	my $o=shift;
	my $devices=$o->{'devices'};
	return scalar @$devices;
}

# Sets/returns the number of e-pucks to handle in parallel.
sub maxparallel {
	my $o=shift;
	$o->{'maxparallel'}=int(shift);
	return $o->{'maxparallel'};
}

# Sets/returns the number of upload attempts.
sub maxattempts {
	my $o=shift;
	$o->{'maxattempts'}=int(shift);
	return $o->{'maxattempts'};
}

# Sets/returns the number of seconds to wait for reset.
sub waitforreset {
	my $o=shift;
	$o->{'waitforreset'}=int(shift);
	return $o->{'waitforreset'};
}

# Sets/returns the number of seconds to wait for reset.
sub belltoreset {
	my $o=shift;
	$o->{'belltoreset'}=int(shift);
	return $o->{'belltoreset'};
}

# Sets/returns the number of seconds to switch on the success light.
sub successlight {
	my $o=shift;
	$o->{'successlight'}=int(shift);
	return $o->{'successlight'};
}

# Opens an INHX32 file.
sub openINHX32 {	# openINHX32($file)
	my $o=shift;
	my $file=shift;
	my $programs=$o->{'programs'};

	# Read file
	my $memorychunks=$o->readfileINHX32($file);
	
	# Create the bootloader entry point
	my $programaddress=$o->entrypoint_startsequence($memorychunks, [0x40, 0x7f, 0x01]);
	$o->entrypoint_returnaddress($memorychunks, 0x17f00*2, $programaddress);

	# Add
	my $packets=$o->createpackets($memorychunks);
	push @$programs, {
		'name'			=> $file,
		'packets'		=> $packets,
		'memorychunks'	=> $memorychunks,
		};
	return;
}

# Opens an INHX32 file with a bootloader helper
sub openINHX32bootloaderhelper {	# openINHX32bootloaderhelper($file)
	my $o=shift;
	my $file=shift;
	my $programs=$o->{'programs'};

	# Read file
	my $memorychunks=$o->readfileINHX32($file);

	# Cut out the bootloader part
	my $memorychunks=$o->memorychunks_subset($memorychunks, 0x17B00*2, 0x100*2);

	# Set the return addresses of the two bootloaders and the start sequence
	$o->entrypoint_startsequence($memorychunks, [0x40, 0x7b, 0x01]);
	$o->entrypoint_returnaddress($memorychunks, 0x17b00*2, [0, 0, 0]);
	$o->entrypoint_returnaddress($memorychunks, 0x17f00*2, [0, 0, 0]);

	# Add
	my $packets=$o->createpackets($memorychunks);
	push @$programs, {
		'name'			=> 'Bootloader helper',
		'id'			=> 'bootloader_helper',
		'packets'		=> $packets,
		'memorychunks'	=> $memorychunks,
		};

	return;
}

# Opens an INHX32 file with a new bootloader
sub openINHX32bootloader {	# openINHX32bootloader($file)
	my $o=shift;
	my $file=shift;
	my $programs=$o->{'programs'};

	# Read file
	my $memorychunks=$o->readfileINHX32($file);

	# Cut out the bootloader part
	my $memorychunks=$o->memorychunks_subset($memorychunks, 0x17F00*2, 0x100*2);
	
	# Set the return addresses of the two bootloaders and the start sequence
	$o->entrypoint_startsequence($memorychunks, [0x40, 0x7f, 0x01]);
	$o->entrypoint_returnaddress($memorychunks, 0x17b00*2, [0, 0, 0]);
	$o->entrypoint_returnaddress($memorychunks, 0x17f00*2, [0, 0, 0]);
	
	# Add
	my $packets=$o->createpackets($memorychunks);
	push @$programs, {
		'name'			=> 'New bootloader',
		'id'			=> 'bootloader_new',
		'packets'		=> $packets,
		'memorychunks'	=> $memorychunks,
		};

	return;
}

# Reads a INHX32 file and creates a memory chunk of the contents.
sub readfileINHX32 {	# $memorychunks = readfileINHX32($file)
	my $o=shift;
	my $file=shift;

	# Prepare
	my $memorychunks=[];

	# Read file
	my $linecc=0;
	my $memory;
	$o->log('Reading file ', $file);
	open(F, $file) || die 'Unable to open file '.$file;
	while (my $line=<F>) {
		while (chomp $line) {$line=$1 if ($line=~/^(.*)\r$/);}
		next if (length($line)<6);

		# Get address
		$line =~ /^\:([0-9a-f]{2,2})([0-9a-f]{2,2})([0-9a-f]{2,2})([0-9a-f]{2,2})([0-9a-f]*)([0-9a-f]{2,2})$/i || next;
		my $datalen=hex($1);
		my $addr=hex($2)*256+hex($3);
		my $type=hex($4);
		my $data=$5;
		my $checksum=hex($6);
		if ($datalen*2!=length($data)) {
			$o->log_warning('Line ', $linecc, ': wrong data length (line ignored)');
			next;
		}

		# Check type of record
		if (($type==0) && ($memory)) {
			# Write data to memory array
			for (my $i=0; $i<$datalen; $i++) {
				$memory->[$addr]=hex(substr($data, $i*2, 2));
				$addr++;
			}
		} elsif ($type==4) {
			# Select the right memory chunk
			my $addr=hex($data);
			if ($addr<4) {
				$addr*=0x10000;
				my $chunk=$o->memorychunks_get($memorychunks, $addr, 0);
				$memory=$chunk->{'memory'};
			} else {
				$memory=undef;
			}
		}
	} continue {
		$linecc++;
	}
	close F;

	return $memorychunks;
}

# Sets the bootloader return address.
sub entrypoint_startsequence {
	my $o=shift;
	my $memorychunks=shift;
	my $address=shift;

	my $chunk=$o->memorychunks_get($memorychunks, 0, 2);
	my $memory=$chunk->{'memory'};
	my $oldaddress=[$memory->[0], $memory->[1], $memory->[4]];

	my @startsequence=($address->[0], $address->[1], 0x04, 0x00, $address->[2], 0x00, 0x00, 0x00);
	for (my $i=0; $i<scalar(@startsequence); $i++) {
		$memory->[$i]=$startsequence[$i];
	}
	return $oldaddress;
}

# Sets the bootloader return address.
sub entrypoint_returnaddress {
	my $o=shift;
	my $memorychunks=shift;
	my $bootloader=shift;
	my $address=shift;

	my $chunk=$o->memorychunks_get($memorychunks, $bootloader, 1);
	my $memory=$chunk->{'memory'};
	$memory->[108]='id_lowbyte';
	$memory->[109]='id_highbyte';
	$memory->[110]=0x00;
	$memory->[111]=0x00;
	$memory->[112]=$address->[0];
	$memory->[113]=$address->[1];
	$memory->[114]=0x04;
	$memory->[115]=0x00;
	$memory->[116]=$address->[2];
	$memory->[117]=0x00;
	$memory->[118]=0x00;
	$memory->[119]=0x00;
	return;
}

# Returns a subset of the memory
sub memorychunks_subset {	# $memorychunks = memorychunks_subset($memorychunks, $start, $len)
	my $o=shift;
	my $memorychunks=shift;
	my $start=shift;
	my $len=shift;

	# Copy the selected memory
	my $newmemorychunks=[];
	foreach my $chunk (@$memorychunks) {
		my $memory=$chunk->{'memory'};
		my $baseaddress=$chunk->{'baseaddress'};

		# Select part
		my $mstart=$start-$baseaddress;
		my $mend=$start+$len-$baseaddress;
		$mstart=0 if ($mstart<0);
		$mend=scalar(@$memory) if ($mend>scalar(@$memory));
		next if ($mend-$mstart<1);

		# Copy memory
		my $newmemory=[];
		for (my $i=$mstart; $i<$mend; $i++) {
			$newmemory->[$i-$mstart]=$memory->[$i];
		}

		# Create a new chunk
		my $newchunk={
			'memory'=>$newmemory,
			'order'=>$chunk->{'order'},
			'baseaddress'=>$baseaddress+$mstart,
			};
		push @$newmemorychunks, $newchunk;
	}

	return $newmemorychunks;
}

# Returns the chunk of a given address.
sub memorychunks_get {
	my $o=shift;
	my $memorychunks=shift;
	my $address=shift;
	my $defaultorder=shift;

	# Return an existing chunk if possible
	foreach my $chunk (@$memorychunks) {
		return $chunk if ($address==$chunk->{'baseaddress'});
	}

	# Otherwise, create a new chunk
	my $chunk={
		'memory'		=> [],
		'baseaddress'	=> $address,
		'order'			=> $defaultorder,
		};

	push @$memorychunks, $chunk;
	return $chunk;
}

# Prepares the packets to be transmitted.
sub createpackets {
	my $o=shift;
	my $memorychunks=shift;

	# Prepare the packet array
	my $packets=[];

	# For each chunk
	my @sorted=sort {$a->{'order'}<=>$b->{'order'}} @$memorychunks;
	foreach my $chunk (@sorted) {
		my $memory=$chunk->{'memory'};
		my $baseaddress=$chunk->{'baseaddress'};

		# Send
		my $addr=0;
		while ($addr<scalar(@$memory)) {
			# Prepare packet
			my $packet=[];
			push @$packets, $packet;

			# Send address
			my $writeaddress=($addr+$baseaddress)/2;
			push @$packet, $writeaddress % 256;
			$writeaddress=int($writeaddress / 256);
			push @$packet, $writeaddress % 256;
			$writeaddress=int($writeaddress / 256);
			push @$packet, $writeaddress % 256;
			push @$packet, 0x60;

			# Send data
			for (my $i=0; $i<32; $i++) {
				push @$packet, (defined $memory->[$addr] ? $memory->[$addr] : 255);
				push @$packet, (defined $memory->[$addr+1] ? $memory->[$addr+1] : 255);
				push @$packet, (defined $memory->[$addr+2] ? $memory->[$addr+2] : 255);
				$addr+=4;
			}
		}
	}

	return $packets;
}

# Adds a device to the list of devices to be programmed.
sub adddevice {
	my $o=shift;
	my $devices=$o->{'devices'};
	my $devicename=shift || return;
	my $id=shift;

	push @$devices, EpuckBootloader::Device->new($o, $devicename, $id);
	return;
}

# Main loop for the upload.
sub send {
	my $o=shift;
	my $devices=$o->{'devices'};
	$|=1;

	# Create packets
	$o->main_ontimeout();

	# Main loop
	my $start=[Time::HiRes::gettimeofday()];
	$o->{'exit'}=0;
	while ($o->{'exit'}==0) {
		# Prepare select
		my $rin;
		foreach my $device (@$devices) {
			my $fh=$device->{'fh'} || next;
			vec($rin, fileno($fh), 1) = 1;
		}

		# Select
		my $cc=select($rin, undef, undef, 0.02);

		# Update the time
		$o->{'now'}=Time::HiRes::tv_interval($start);

		# If at least one byte is available, read
		if ($cc) {
			foreach my $device (@$devices) {
				my $fh=$device->{'fh'} || next;
				if (vec($rin, fileno($fh), 1)) {
					my $cc=sysread($fh, my $input, 1);
					$device->oninput($input);
				}
			}
		}

		# Timeout
		foreach my $device (@$devices) {
			next if (! $device->{'timeout'});
			if ($o->{'now'}>=$device->{'timeout'}) {
				$device->{'timeout'}=0;
				$device->oninput(undef);
			}
		}

		# Main handler
		if ($o->{'timeout'}) {
			if ($o->{'now'}>=$o->{'timeout'}) {
				$o->{'timeout'}=0;
				$o->main_ontimeout();
			}
		}
	}
}

# This function is called every now and then.
sub main_ontimeout {
	my $o=shift;
	my $devices=$o->{'devices'};
	$o->{'timeout'}=$o->{'now'}+1;

	# Statistics
	my $ccactive=0;
	foreach my $device (@$devices) {
		$ccactive++ if ($device->{'fh'});
	}

	# If some devices are active, ok
	return if ($ccactive>0);

	# Otherwise, open J new connections
	my @sorted=sort {($a ? $a->{'openattempts'} : 100) <=> ($b ? $b->{'openattempts'} : 100)} @$devices;
	foreach my $device (@sorted) {
		next if ($device->{'done'});
		next if ($device->{'fh'});
		$ccactive++ if ($device->open());
		last if ($ccactive >= $o->{'maxparallel'});
	}

	# If none are active, quit
	if ($ccactive==0) {
		$o->{'exit'}=1;
		return;
	}

	# Bell to indicate that it starts now
	print "\a" if ($o->{'belltoreset'});
	return;
}

# Writes all packets to a file.
sub writepackets {
	my $o=shift;
	my $file=shift || return 0;
	my $programs=$o->{'programs'};

	foreach my $program (@$programs) {
		my $curfile=$file;
		$curfile.='.'.$program->{'id'} if ($program->{'id'});
		my $packets=$program->{'packets'};

		open(my $fh, '>', $curfile) || return 0;
		foreach my $packet (@$packets) {
			print $fh join('', map {&readablebyte($_).' '} @$packet), "\n";
		}
		close $fh;
	}
	return 1;
}

# Writes the memory chunks in binary representation to different files.
sub writefilebin {	# writefilebin($file)
	my $o=shift;
	my $file=shift;
	my $programs=$o->{'programs'};

	foreach my $program (@$programs) {
		my $curfile=$file;
		$curfile.='.'.$program->{'id'} if ($program->{'id'});
		my $memorychunks=$program->{'memorychunks'};

		my @sorted=sort {$a->{'order'}<=>$b->{'order'}} @$memorychunks;
		foreach my $chunk (@sorted) {
			my $memory=$chunk->{'memory'};
			my $baseaddress=$chunk->{'baseaddress'};

			open(F, '>', $curfile.'.baseaddress'.$baseaddress) || return 0;
			for (my $i=0; $i<scalar(@$memory); $i++) {
				print F chr((defined $memory->[$i] ? $memory->[$i] : 255));
			}
			close F;
		}
	}

	return 1;
}

# Writes the memory chunks in hexadecimal representation to a file.
sub writefilehex {	# writefilehex($file)
	my $o=shift;
	my $file=shift;
	my $programs=$o->{'programs'};

	foreach my $program (@$programs) {
		my $curfile=$file;
		$curfile.='.'.$program->{'id'} if ($program->{'id'});
		my $memorychunks=$program->{'memorychunks'};

		open(F, '>', $curfile) || return 0;
		my @sorted=sort {$a->{'order'}<=>$b->{'order'}} @$memorychunks;
		foreach my $chunk (@sorted) {
			my $memory=$chunk->{'memory'};
			my $baseaddress=$chunk->{'baseaddress'};

			print F 'Base address: ', $baseaddress, ' (length: ', scalar(@$memory), ' bytes)';
			for (my $i=0; $i<scalar(@$memory); $i++) {
				if ($i % 16 == 0) {print F "\n";}
				print F &readablebyte((defined $memory->[$i] ? $memory->[$i] : 255)), ' ';
			}
			print F "\n";
		}
		close F;
	}

	return 1;
}

# Converts a number to a two-digit hex representation.
sub tohex2 {	# $hex = tohex2($int)
	my $var=shift;
	my $var1=$var/16;
	my $var2=$var % 16;
	my @arr=(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 'A', 'B', 'C', 'D', 'E', 'F');
	return $arr[$var1].$arr[$var2];	
}

sub readablebyte {	# $t = readablebyte($byte)
	my $byte=shift;
	return sprintf('%02X', $byte) if ($byte eq int($byte));
	return '<'.$byte.'>';
}

sub makereadable {
	my $t=shift;
	$t=~s/([\x00-\x1F])/'<'.sprintf('%02X', ord($1)).'>'/eg;
	$t=~s/([\x80-\xFF])/'<'.sprintf('%02X', ord($1)).'>'/eg;
	return $t;
}

sub log_line {
	my $o=shift;
	my @tt=localtime();
	$tt[0]='0'.$tt[0] if ($tt[0]<10);
	$tt[1]='0'.$tt[1] if ($tt[1]<10);
	$tt[2]='0'.$tt[2] if ($tt[2]<10);
	print '[', $tt[2], ':', $tt[1], ':', $tt[0], '] ', @_, "\n";
	return;
}

sub log {
	my $o=shift;
	my $t=join('', @_);
	$t=&makereadable($t);
	$o->log_line($t);
	return;
}

sub log_error {
	my $o=shift;
	my $t=join('', @_);
	$t=&makereadable($t);
	$o->log_line('ERROR: ', $t);
	return;
}

sub log_warning {
	my $o=shift;
	my $t=join('', @_);
	$t=&makereadable($t);
	$o->log_line('WARNING: ', $t);
	return;
}

# This class implements one epuck device.
package EpuckBootloader::Device;

# Creates a new object.
sub new {	# new()
	my $o=shift;
	my $class=(ref($o) || $o);
	my $newo=bless {}, $class;
	$newo->init(@_);
	return $newo;
}

# Initializes the object.
sub init {	# init($ebl, $name, $id)
	my $o=shift;
	$o->{'ebl'}=shift;
	$o->{'name'}=shift;
	$o->{'id'}=shift;
	return;
}

# Opens the device.
sub open {
	my $o=shift;
	my $devicename=$o->{'name'};
	my $ebl=$o->{'ebl'};

	# Fail after some attempts
	if ($o->{'openattempts'} >= $ebl->{'maxattempts'}) {
		$o->{'err'}='err_toomanyattempts';
		$o->{'done'}=1;
		$o->log_warning('Giving up on serial port (after ', $ebl->{'maxattempts'}, ' unsuccessful attempts)');
		return 0;
	}

	# Show message
	$o->{'openattempts'}++;
	if ($o->{'openattempts'}>1) {
		$o->log('Connecting to ', $devicename, ' (attempt ', $o->{'openattempts'}, ')');
	} else {
		$o->log('Connecting to ', $devicename, ' (robot ID ', $o->{'id'}, ')');
	}

	# Try to connect
	undef $!;
	if (open(my $fh, '+<', $devicename)) {
		system('stty 115200 min 1 time 5 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke < '.$devicename);
		my $oldfh=select($fh); $|=1; select($oldfh);

		$o->{'fh'}=$fh;
		$o->setstate('OPEN', 0.1);
		return 1;
	}

	# On failure, try again later
	$o->log_warning('Unable to open serial port: ', $!, ' (trying again later)');
	return 0;
}

# This method is called to process a char (or a timeout).
sub oninput {
	my $o=shift;
	my $input=shift;

	my $state=$o->{'state'};
	if ($state eq 'OPEN') {
		$o->startnextprogram();
	} elsif ($state eq 'RESET_WAIT') {
		if ($input eq 'q') {
			$o->setstate('RESET_WAIT_K', 1);
		} else {
			$o->sendbootloaderstart();
		}
	} elsif ($state eq 'RESET_WAIT_K') {
		if (! defined $input) {
			$o->sendbootloaderstart();
		} elsif ($input eq 'K') {
			$o->transmitnextpacket();
		} elsif ($input eq 'q') {
			$o->setstate('RESET_WAIT_K', 1);
		} else {
			$o->sendbootloaderstart();
		}
	} elsif ($state eq 'WAITACK') {
		if (! defined $input) {
			$o->log_error($o->{'name'}, ': Transmission failed (robot did not ack packet) '.$input);
			$o->close('err_ack');
		} elsif ($input eq 'K') {
			$o->transmitnextpacket();
		} else {
			print "\n";
			$o->log_error('Transmission failed (robot did not ack packet)');
			$o->close('err_ack');
		}
	}

	return;
}

sub startnextprogram {
	my $o=shift;
	my $fh=$o->{'fh'};

	# Check if there is a new program to upload
	my $programs=$o->{'ebl'}->{'programs'};
	my $program=$programs->[$o->{'nextprogramid'}];
	return $o->close() if (! $program);
	$o->{'currentprogram'}=$program;
	$o->log('Uploading ', $program->{'name'});

	# Reset counters
	$o->{'bootloaderstart'}=$ebl->{'now'};
	$o->{'bootloaderlaststartchar'}=$ebl->{'now'};
	$o->{'nextpacketid'}=0;

	# Send software reset sequence
	print 'R';
	print $fh 'R';
	$o->setstate('RESET_WAIT', 0.1);
	return;
}

sub sendbootloaderstart {
	my $o=shift;
	my $fh=$o->{'fh'};
	my $ebl=$o->{'ebl'};

	# If we sent the start char many times and still didn't get an answer, there's probably a problem
	if ($ebl->{'now'}-$o->{'bootloaderstart'} >= $ebl->{'waitforreset'}) {
		$o->log_error('Bootloader on PIC did not reply (0x71 0x4B expected)');
		$o->close('err_bootloaderstart');
		return;
	}

	# Set timeout
	$o->setstate('RESET_WAIT', 0.1);

	# If we just sent a C1, don't send it again
	return if ($ebl->{'now'}-$o->{'bootloaderlaststartchar'} < 0.05);
	
	# Send start char
	print '.';
	print $fh chr(0xC1);
	return;
}

sub transmitnextpacket {
	my $o=shift;
	my $fh=$o->{'fh'};
	my $program=$o->{'currentprogram'};

	# Get next packet
	my $packets=$program->{'packets'};
	my $packet=$packets->[$o->{'nextpacketid'}];
	return $o->sendendoftransmission() if (! $packet);
	$o->{'nextpacketid'}++;

	# Prepare packet
	my $sendpacket=$o->preparepacket($packet);

	# Send packet
	print '*';
	print $fh join('', map {chr($_)} @$sendpacket);

	$o->setstate('WAITACK', 1);
	return;
}

sub preparepacket {
	my $o=shift;
	my $packet=shift || return;

	# Finalize packet
	my $sendpacket=[];
	foreach my $byte (@$packet) {
		my $newbyte=$byte;
		$newbyte=$o->{'id'} % 256 if ($byte eq 'id_lowbyte');
		$newbyte=$o->{'id'} / 256 if ($byte eq 'id_highbyte');
		push @$sendpacket, $newbyte;
	}

	# Calculate and add checksum
	my $checksum=0;
	foreach my $byte (@$sendpacket) {
		$checksum+=$byte;
	}
	$checksum=$checksum % 256;
	push @$sendpacket, ($checksum==0 ? 0 : 256-$checksum);

	return $sendpacket;
}

sub sendendoftransmission {
	my $o=shift;
	my $fh=$o->{'fh'};
	my $ebl=$o->{'ebl'};
	my $program=$o->{'currentprogram'};

	# Check if there is another program to upload
	$o->{'nextprogramid'}++;
	my $programs=$o->{'ebl'}->{'programs'};
	my $nextprogram=$programs->[$o->{'nextprogramid'}];

	# Send the final packet
	my $duration=int($ebl->{'successlight'}/13.4*1000+0.5);
	$duration=1 if ($nextprogram);
	my $packet=[];
	push @$packet, $duration % 256;
	$duration=int($duration / 256);
	push @$packet, $duration % 256;
	push @$packet, 0x0;
	push @$packet, 0xFF;
	print $fh join('', map {chr($_)} @$packet);

	# Print that we're done
	print "\n";
	$o->log('Transmission of ', $program->{'name'}, ' complete!');

	# If there no other program to upload, close
	return $o->close() if (! $nextprogram);

	# Otherwise wait 1 second for the bootloader to reboot and go on
	return $o->setstate('OPEN', 1);
}

sub close {
	my $o=shift;
	my $err=shift;

	# Close and reset the state
	close $o->{'fh'} if ($o->{'fh'});
	delete $o->{'fh'};
	delete $o->{'state'};
	delete $o->{'timeout'};

	# Save a status message
	if ($err) {
		$o->{'error'}=$err;
	} else {
		$o->{'done'}=1;
	}
	return;
}

# Sets the state of the device
sub setstate {
	my $o=shift;
	$o->{'state'}=shift;
	$o->{'timeout'}=$o->{'ebl'}->{'now'}+shift;
	return;
}

sub log {
	my $o=shift;
	return $o->{'ebl'}->log('[', $o->{'name'}, '] ', @_);
}

sub log_error {
	my $o=shift;
	return $o->{'ebl'}->log('[', $o->{'name'}, '] ERROR: ', @_);
}

sub log_warning {
	my $o=shift;
	return $o->{'ebl'}->log('[', $o->{'name'}, '] WARNING: ', @_);
}

1;
