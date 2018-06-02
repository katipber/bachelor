package com.katipber.connect4; 

import controller.GameController;

import javafx.application.Application;
import static javafx.application.Application.launch;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundImage;
import javafx.scene.layout.BackgroundPosition;
import javafx.scene.layout.BackgroundRepeat;
import javafx.scene.layout.BackgroundSize;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;


public class MainApp extends Application {
    
    public static final int COL  = 7;
    public static final int ROW  = 6;
    public static final int PAD  = 10;
    public static final int SIZE = 80;
    
    @Override
    public void start(Stage stage) throws Exception {
        
        GameController gc = new GameController(stage);
        
        Image image = new Image("file:img\\c4.jpg", 900, 600, false, true);
        BackgroundImage backgroundImage = new BackgroundImage(image, BackgroundRepeat.NO_REPEAT, BackgroundRepeat.NO_REPEAT, BackgroundPosition.CENTER, BackgroundSize.DEFAULT);
        
        BorderPane root = new BorderPane();
        root.setBackground(new Background(backgroundImage));
        root.setLeft(gc.getMenu());
        root.setCenter(gc.getController());
        root.setRight(null);
        
        Scene scene = new Scene(root, 900, 600);
        stage.setScene(scene);
        stage.show();
   }

    public static void main(String[] args) {
        launch(args);
    }
}
