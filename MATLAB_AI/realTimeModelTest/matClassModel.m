function prediction = matClassModel(y) %#codegen
    loadModel = load("adam_multclass_403_2.mat");
    netTrained = loadModel.netTrained;
    classes = categorical({'Dawsons'; 'HolyStone'; 'NoDrone'});
    X = HelperMelSpectrograms(y,48000);
    dlX = dlarray(X,'SSC');
    scores = predict(netTrained,dlX);
    output = extractdata(scores);
    disp(output)
    if (output(1) > output(2)) && (output(1) > output(3))
        prediction = classes(1);
    elseif (output(2) > output(1)) && (output(2) > output(3))
        prediction = classes(2);
    else
        prediction = classes(3);
    end
end