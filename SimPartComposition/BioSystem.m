%
% class BioSystem
%
classdef BioSystem < handle
    
    properties
        parts       = [];   % parts in this BioSystem
        compositors = [];   % compositors in this BioSystem
        map_compositors     % a mapping between compositor name and actual compositor
        
    end
    
    methods
        function B = BioSystem()
            % class constructor
        end
        
        %
        % AddCompositor
        %
        function self = AddCompositor(self, new_compositor)
            self.compositors = [self.compositors; new_compositor];
        end
        
        function self = AddPart(self, new_part)
            self.parts = [self.parts; new_part];
        end
        
        %
        % run simulation
        %
        function [T, Y] = run(self, tspan)
            % first, set up compositor mapping
            self.map_compositors = containers.Map;
            for k=1:length(self.compositors)
                self.map_compositors(self.compositors(k).name) = k;                
            end
           
            % initial values
            y0 = [];
            for i = 1:length(self.compositors)
                y0 = [y0 self.compositors(i).v];
            end                    
            
            % determine rates for all compositors
            for i = 1:length(self.parts)
                p = self.parts(i);
                for k = 1:length(p.compositors)
                    p.compositors(k).AddRate(p.rates(k));
                end
            end
            
            % debug: print all compositor rates
            for j=1:length(self.compositors)
                disp(self.compositors(j).r)
            end
            
            [T,Y] = ode23s(@self.sys_ode,tspan,y0);
            
        end
       
        %
        % ODE of system
        %
        function dy = sys_ode(self,t, y)
            dy = zeros(length(y), 1);
            % first, store new values in system compositors
            for i = 1:length(y)
                self.compositors(i).v = y(i);
            end
            
            % next, evaluate dy for each compositor
            for i = 1:length(y)
                dy(i) = eval(self.compositors(i).r);
            end            
        end 
    end
end