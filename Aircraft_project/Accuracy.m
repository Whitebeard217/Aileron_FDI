% Read data from Excel file
filename = 'Fault_marker_data.xlsx'; % change to your filename
data = readmatrix(filename);

ground_truth = data(:,1);
prediction = data(:,2);

% Check correctness for each sample
correct = ground_truth == prediction;

% Mean accuracy
mean_accuracy = mean(correct);

fprintf('Mean accuracy: %.4f\n', mean_accuracy);
