use mydb;
create table Musician
(
	Musician_ID int not null primary key identity(1,1),
	Name varchar(100) not null,
	Instrument varchar(100),
	Born varchar(100),
	Country varchar(100)
);

create table Label(
	Label_ID int not null primary key identity(1,1),
	Name varchar(100) not null,
	Holder varchar(100) not null,
	Foundation varchar(100),
	WWW varchar(100)
);

create table Band
(
	Band_ID int not null primary key identity(1,1),
	Name varchar(100) not null,
	Leader_ID int not null foreign key (leader_id) references Musician(musician_id) on delete cascade,
	Label_ID int null foreign key (Label_ID) references label(label_id) on delete set null,
	Style varchar(100)
);
create table Album
(
	Album_ID int not null primary key identity(1,1),
	Name varchar(100) not null,
	Band_ID int null foreign key (band_id) references band(band_id) on delete set null,
	Year_album int, check (year_album>0), 
	Num_song int check (num_song>0)
);
create table MusInBand
(
	Musician_ID int not null foreign key (musician_id) references Musician(musician_id),
	Band_ID int not null foreign key (band_id) references Band(Band_id),
	Begining_cotr date,
	Ending_contr date,
	primary key(Musician_ID, Band_ID),
);

create procedure musician_info
@st varchar(100)
as
begin
	select m.Name,b.Name,b.Style,l.Name
	from Musician as m,Band as b,Label as l,MusInBand as mib
	where   m.Name=@st and
			mib.musician_id=m.musician_id and
			b.band_id=mib.band_id and
			l.label_id=b.label_id;
end;

create procedure label_
@st varchar(100)
as
	begin
		select b.Name,COUNT(b.Name) as NumMember
		from Band as b,MusInBand as mib,Label as l
		where l.name=@st and
			  b.label_id=l.label_id and
			  mib.Band_ID = b.band_id group by b.Name;
	end;	

insert into Musician values('Tarkov','guitar','1991','Russia');
insert into Musician values('Tavor','bas guitar','1992','USA');
insert into Musician values('Mark','voice','1989','Canada');
insert into Musician values('Kerk','drums','1967','France');
insert into Musician values('Orc','keyboard','1976','Russia');
insert into Musician values('Elf','guitar','1978','Russia');

insert into Label values('Leningrad','Shnur','1978','www.leningrad.ru');
insert into Label values('lable#2','Ya','1965','www.chto-to.com');
insert into Label values('lable#5','google','200','www.gooooogle.com');
insert into Label values('lable#8','uha','1989','www.fish.com');
insert into Label values('lable#1','Taras Bulba','1976','www.ukr.com');
insert into Label values('lable#0','Nemo','1998','www.league.com');

insert into Band values('Eziki',1,1,'rock');
insert into Band values('roziki',2,5,'pop');
insert into Band values('la-la-la',3,2,'folk');
insert into Band values('band#4',4,3,'hardcore');

insert into Album values('albom_1',1,1995,10);
insert into Album values('albom_2',1,1997,13);
insert into Album values('albom_3',1,1999,12);
insert into Album values('albom_4',1,2000,11);
insert into Album values('eto mi',2,1987,10);
insert into Album values('oni uze tut',2,1989,11);
insert into Album values('bulichev',2,1989,9);
insert into Album values('gore ot uma',4,200,3);
insert into Album values('HVV',4,1978,7);

insert into MusInBand values(1,1,'2000-01-01' ,'2010-01-01');
insert into MusInBand values(2,2,'1978-02-02' ,'2000-01-01');
insert into MusInBand values(3,3,'1990-03-02' ,'1995-01-01');
insert into MusInBand values(4,4,'1978-06-06' ,'1979-01-01');
insert into MusInBand values(5,1,'1967-08-21' ,'1969-01-01');
insert into MusInBand values(6,2,'1990-11-11' ,'1995-01-01');