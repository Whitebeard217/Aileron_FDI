% Time vector from 0 to 13 with 0.5 second increments
t = 0:0.5:13;

% Generate random control points
num_control_points = 5;
control_times = [0, sort(randsample(2:12, num_control_points-2)), 13];
control_deflections = 14 * (2 * rand(1, num_control_points) - 1);

% Ensure smooth start and end
control_deflections(1) = 0;
control_deflections(end) = 0;

% Generate smooth curve using spline interpolation
deflection = spline(control_times, control_deflections, t);

% Add small random variations
small_variations = 0.5 * sin(2*pi*0.5*t) .* (2*rand(size(t))-1);
deflection = deflection + small_variations;

% Ensure deflection stays within -14 to 14 range
deflection = max(min(deflection, 14), -14);

% Create timeseries object
ts = timeseries(deflection, t);
ts.Name = 'Aileron Deflection';
ts.TimeInfo.Units = 'seconds';
ts.DataInfo.Units = 'degrees';

% Save the timeseries object to the workspace
assignin('base', 'aileron_deflection_ts', ts);

% Display some statistics
disp(['Min deflection: ' num2str(min(deflection)) ' degrees'])
disp(['Max deflection: ' num2str(max(deflection)) ' degrees'])


% Plot the time series
plot(ts)
title('Aileron Deflection During Banking Maneuver')
xlabel('Time (seconds)')
ylabel('Deflection Angle (degrees)')
grid on