%file repressilator.m
%Model of repressilator oscillatory genetic circuit
%from Elowitz and Leibler (2000) Nature 403, pp 335-338.
%Figure 7.21

function repressilator

clear all

%declare model parameters
global alpha
global alpha0
global n
global beta

%assign parameter values
alpha0=60*(5e-4); %transcripts/sec
alpha=60*(5-alpha0);  %transcripts/sec
n=2;
beta=1/5;

%set simulation parameters
Tend=1000
ODEFUN=@repressilatorddt;
options=odeset('Refine', 6);

%set initial condition: state = [m1 p1 m2 p2 m3 p3]
x0=[0.2  0.1, 0.3 0.1 0.4 0.5]';

%run simulation
[t,s]=ode45(ODEFUN, [0,Tend], x0, options);

%produce figure 7.21A
figure(1)
set(gca,'fontsize',14)
t=t/0.3485
plot(t,40*s(:,2), 'k',t,40*s(:,4), 'k--', t,40*s(:,6), 'k:','Linewidth', 3)
axis([0 800 0 7000])
ylabel('protein abundance (molecules per cell)')
xlabel('Time (min)')
legend('Protein 1', 'Protein 2', 'Protein 3')

%bifurcation analysis
%this is a brute-force bifurcation analysis, analysing system behaviour
%over a mesh of points.  The analysis involves checking the eigenvalues of
%the steady state.  A derivation of the criterion can be found in the book
%"Physical Biology of the Cell", by Phillips, Kondev, and Theriot (Garland
%Science, 2009, on pages 754-757.

%mesh size
N=1000;

%mesh definition
a=zeros(1,N);
bon=zeros(1,N);
boff=zeros(1,N);
x=zeros(1,N);
xthresh=zeros(1,N);

figure(99)
hold on

%three cases for values of n
for figcase=1:3
    
    if figcase==1
        n=1.7;
    else if figcase==2
            n=2;
        else if figcase==3
                n=2.5;
            end
        end
    end
          

error=0;
for i=1:N
        alpha=60*(10^[5*(i-1)/N]-alpha0)/60; %set value of alpha
        a(i)=alpha;
        
        myfun1 = @(p) abs(p-alpha/(1+p^n) + alpha0); %steady-state condition
        [pstar,perror]=fminsearch(myfun1, 0); %determine steady state
        xstar=-alpha*n*pstar^(n-1)/(1+pstar^n)^2; %eigenvalue for testing
        xthresh(i)=(4+2*xstar)/(3*xstar^2); %eigenvalue criterion
        x(i)=xstar;
        if perror>0.1
            error=1;
        end
        
        if error==0
            
        if xthresh(i)>=0.25
            bon(i)=inf;
            boff(i)=inf;
        else
            
            %search over b values
        btest=10^(-10);
        bonflag=0;
        while (bonflag==0)&&(btest<10^12)
            if btest/(btest+1)^2 > xthresh(i) %test eigenvalue crtierion
                bonflag=1;
            else
                btest=btest*10^(20/N);
            end
            
        end
        bon(i)=btest;
        
        boffflag=0;
        
        bonflag=0;
        while (boffflag==0)&&(btest<10^12)
            if btest/(btest+1)^2 < xthresh(i)
                boffflag=1;
            else
                btest=btest*10^(20/N);
            end
            
        end
        boff(i)=btest;
        
        end
        end

end

%produce figure 7.21B
figure(2)

if figcase==1
loglog(a,bon, 'k--',a,boff, 'k--', 'Linewidth',3)
else if figcase==2
        loglog(a,bon, 'k-.',a,boff, 'k-.', 'Linewidth',3)
    else if figcase==3
            loglog(a,bon, 'k',a,boff, 'k', 'Linewidth',3)
        end
    end
end
hold on
end

axis([10^-1 10^5 10^-1 10^5])
set(gca,'fontsize',14)
xlabel('\alpha (scaled maximal expression rate) ')
ylabel('\beta (ratio of protein to mRNA decay rates)')
h=text(0.5,2, 'stable steady state', 'Fontsize', 18)
set(h, 'rotation',90)

end

%model dynamics
function dS=repressilatorddt(t,s)


global alpha
global alpha0
global n
global beta

m1=s(1);
p1=s(2);
m2=s(3);
p2=s(4);
m3=s(5);
p3=s(6);

m1ddt= alpha0 + alpha/(1+p3^n) - m1;
p1ddt=beta*(m1-p1);
m2ddt= alpha0 + alpha/(1+p1^n) - m2;
p2ddt=beta*(m2-p2);
m3ddt= alpha0 + alpha/(1+p2^n) - m3;
p3ddt=beta*(m3-p3);   

dS =[m1ddt,  p1ddt, m2ddt, p2ddt, m3ddt, p3ddt]';

end
