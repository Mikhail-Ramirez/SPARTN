function prediction = matClassModel(y) %#codegen
    loadModel = load("classifyDroneModel.mat");
    netTrained = loadModel.netTrained;
    classes = categorical({'Dawsons'; 'HolyStone'; 'NoDrone'});
    Y = repmat(y(:), 1, 2);
    X = HelperMelSpectrograms(Y,44100);
    dlX = dlarray(X,'SSC');
    scores = predict(netTrained,dlX);
    output = extractdata(scores);
    if (output(1) > output(2)) && (output(1) > output(3))
        prediction = classes(1);
    elseif (output(2) > output(1)) && (output(2) > output(3))
        prediction = classes(2);
    else
        prediction = classes(3);
    end
end