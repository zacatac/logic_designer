%{

This is the simple enzyme catalysis example we did in lecture on 2013-09-19
Things to note: you should save this script in the same directory as the
  other scripts used (BioSystem.m, Compositor.m, Rate.m, Part.m)

%}

%Define your constants
tspan = 0:0.01:3; % start time : interval : end time


%Name your system
%Format: systemName=BioSystem();
sys1 = BioSystem();


%Initialize your compositors, set initial conc for each species
%Format: Compositor('speciesName',initial conc);
E1 = Compositor('E1',10); %initial state
M1 = Compositor('M1',10); %initial state
M2 = Compositor('M2',0);  %initial state


%Add compositors to your system
%Format: systemName.AddCompositor('speciesName')
sys1.AddCompositor(E1);
sys1.AddCompositor(M1);
sys1.AddCompositor(M2);


%Define your parts
%Format: partName=Part('partName',[compositor matrix],[rate matrix])
%   compositor matrix defines the order of species for the part
%   rate matrix must be in the same arrangement as compositor matrix
%   rate matrix format is [Rate('expr1') Rate('expr1') ...] 
P1 = Part('M1-(E1)->M2',[M1;E1;M2],...
    [Rate('(-10 * val(E1) * val(M1)) / (5+val(M1))')
    Rate('0')
    Rate('(10 * val(E1) * val(M1)) / (5 + val(M1))')]);


%Add parts to your system
%Format: systemName.AddPart(partName);
sys1.AddPart(P1);


%Run simulation for your time span
%Format: [time,outputs] = systemName.run(timeSpan)
[T,Y]=sys1.run(tspan);


%Plot your results (there are lots of options for doing this)
plot(T,Y)
legend('E1','M1','M2')
xlabel('Time')
ylabel('Conc')