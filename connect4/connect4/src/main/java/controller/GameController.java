package controller;

import static com.katipber.connect4.MainApp.COL;
import static com.katipber.connect4.MainApp.PAD;
import static com.katipber.connect4.MainApp.SIZE;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Writer;
import view.GameView;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Modality;
import javafx.stage.Stage;

public class GameController {
    
    private final Stage primaryStage;
    private final VBox menu;
    private final Pane controlRoot;
    private final GameView gv;
    private boolean turn;
    private int[][] grid;
    private boolean gameOn;
    private List<Integer> moves;
    
    public GameController(Stage stage) {
        
        turn = true;
        gameOn = true;
        primaryStage = stage;
        gv = new GameView();
        grid = new int[6][7];
        moves = new ArrayList<>();
        
        File saved = new File("saved");
        if (!saved.exists()) {
            try {
                saved.mkdir();
            } catch (Exception e) {
            }
        }
        
        // setting Menu
        Button newGame  = new Button("New Game");
        Button saveGame = new Button("Save Game");
        Button loadGame = new Button("Load Game");
        Button quitGame = new Button("Quit Game");
        
        newGame.setOnAction(e -> newGame());
        saveGame.setOnAction(e -> saveGame());
        loadGame.setOnAction(e -> loadGame());
        quitGame.setOnAction(e -> quitGame());
        
        menu = new VBox(PAD);
        menu.getChildren().addAll(newGame, saveGame, loadGame, quitGame);
        menu.setPadding(new Insets(4 * PAD, 4 * PAD, 4 * PAD, 4 * PAD));
        menu.setAlignment(Pos.BOTTOM_LEFT);
        
        // setting Controller
        controlRoot = new Pane();
        List<TokenController> controller = new ArrayList<>();
        
        for (int i = 0; i < COL; ++i) {
            TokenController tc = new TokenController(i);
            tc.setTranslateX(i * (SIZE + PAD) + 6 * PAD);
            
            tc.setOnMouseClicked(e -> placeToken(tc.col));
            controller.add(tc);
        }
        
        controlRoot.getChildren().add(gv.getGrid());
        
        controlRoot.getChildren().addAll(controller);
        controlRoot.setTranslateY(2 * PAD);
    }
    
    private void loadGame() {
        
        File file;
        file = new File("saved");
        String[] paths;
        ChoiceBox<String> box = new ChoiceBox();

        Stage stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setTitle("Load Game");
        
        Label label = new Label("Select File:");
        Label rez = new Label("");
        
        Button cancelBtn = new Button("Close");
        Button loadBtn = new Button("Load");
        
        cancelBtn.setOnAction(e -> stage.close());
        loadBtn.setOnAction(e -> {
            rez.setText(loadFile(box.getValue()));
            rez.setTextFill(Color.RED);
        });        
        try {
            paths = file.list();
            box.getItems().addAll(paths);
        
        
        } catch (Exception e) {
        }
         
        GridPane pane = new GridPane();
        pane.setAlignment(Pos.CENTER);
        pane.setHgap(PAD);
        pane.setVgap(PAD);
        pane.setPadding(new Insets(2 * PAD, 2 * PAD, 2 * PAD, 2 * PAD));
        
        HBox hbBtn = new HBox(PAD);
        hbBtn.setAlignment(Pos.BOTTOM_RIGHT);
        hbBtn.getChildren().addAll(loadBtn, cancelBtn);
        
        pane.add(label, 0, 0);
        pane.add(box, 1, 0);
        pane.add(rez, 0, 1);
        pane.add(hbBtn, 1, 1);
        
        Scene scene = new Scene(pane, 400, 250);
        stage.setScene(scene);
        stage.showAndWait();
    }
    
    private String loadFile(String file) {
        String rez = "Game Loaded!";
        boolean tmp;
        
        try {
            File f = new File("Saved\\" + file);
            Scanner s = new Scanner(f);
            
            newGame();
            
            tmp = (s.nextInt() == 1 ? true : false);
            turn = (s.nextInt() == 1 ? true : false);
            
            while (s.hasNextInt())
                placeToken(s.nextInt());
            
            gameOn = tmp;
            
        } catch (Exception e) {
            rez = "Loading Failed!";
        }
        
        return rez;
    }
    
    private void saveGame() {
        Stage stage = new Stage();
        stage.initModality(Modality.APPLICATION_MODAL);
        stage.setTitle("Save Game");
        
        Label label = new Label("Enter file name:");
        Label rez = new Label("");
        
        TextField fileName = new TextField();
        fileName.setPromptText("File name");
        
        Button cancelBtn = new Button("Close");
        Button saveBtn = new Button("Save");
        
        cancelBtn.setOnAction(e -> stage.close());
        saveBtn.setOnAction(e -> {
            rez.setText(saveToFile(fileName.getCharacters()));
            rez.setTextFill(Color.RED);
        });
        
        GridPane pane = new GridPane();
        pane.setAlignment(Pos.CENTER);
        pane.setHgap(PAD);
        pane.setVgap(PAD);
        pane.setPadding(new Insets(2 * PAD, 2 * PAD, 2 * PAD, 2 * PAD));
        
        HBox hbBtn = new HBox(PAD);
        hbBtn.setAlignment(Pos.BOTTOM_RIGHT);
        hbBtn.getChildren().addAll(saveBtn, cancelBtn);
        
        pane.add(label, 0, 0);
        pane.add(fileName, 1, 0);
        pane.add(rez, 0, 1);
        pane.add(hbBtn, 1, 1);
        
        Scene scene = new Scene(pane, 400, 250);
        stage.setScene(scene);
        stage.showAndWait();
    }
    
    private String saveToFile(CharSequence c) {
        String rez = "Game Saved!";
        try {
            File f = new File("Saved\\" + c.toString());
            FileWriter fw = new FileWriter(f);
            Writer out = new BufferedWriter(fw);
            
            out.write((gameOn ? "1" : "0") + " " + (turn ? "1" : "0") + "\n");
            moves.forEach((i) -> {
                try {
                    out.write(i.toString() + " ");
                    System.out.println(i.toString() + " ");
                } catch (IOException ex) {
                }
            });
            out.close();
        } catch (Exception e) {
            rez = "Saving Failed!";
        }
        
        return rez;
    }
    
    private void newGame() {
        grid = new int[6][7];
        gv.clearGrid();
        moves.clear();
        turn = true;
        gameOn = true;
    }
    
    private void placeToken(int col) {
        if (!gameOn) return;
        int row = validMove(col);
        if (row < 0)
            return;
        gv.placeToken(gv.makeToken(turn), row, col);
        grid[row][col] = (turn ? 1 : -1);
        moves.add(col);
        if (gameOver(row, col)) {
            gameOn = false;
            System.out.println((turn ? "Red" : "Yellow") + " wins!");
            return;
        }
        turn = !turn;
    }
    
    private boolean gameOver(int r, int c) {
        int cnt;
        // check horizontal
        cnt = 0;
        for (int i = (c > 2 ? c - 3 : 0); i < (c < 4 ? c + 4 : 7); ++i) {
            if (grid[r][i] == (turn ? 1 : -1))
                cnt++;
            else
                cnt = 0;
            if (cnt == 4)
                return true;
        }
        
        // check vertical
        cnt = 0;
        for (int i = (r > 2 ? r - 3 : 0); i < (r < 3 ? r + 4 : 6); ++i) {
            if (grid[i][c] == (turn ? 1 : -1))
                cnt++;
            else
                cnt = 0;
            if (cnt == 4)
                return true;
        }
        
        // check diagonalL
        cnt = 0;
        for (int i = -3; i < 4; ++i) {
            if (inRange(r + i, c + i) && grid[r + i][c + i] == (turn ? 1 : -1))
                cnt++;
            else
                cnt = 0;
            if (cnt == 4)
                return true;
        }
        
        // check diagonalR
        cnt = 0;
        for (int i = -3; i < 4; ++i) {
            if (inRange(r - i, c + i) && grid[r - i][c + i] == (turn ? 1 : -1))
                cnt++;
            else
                cnt = 0;
            if (cnt == 4)
                return true;
        }

        return false;
    }
    
    private boolean inRange(int r, int c) {
        return r >= 0 && r <= 5 && c >= 0 && c <= 6; 
    }
    
    private int validMove(int c) {
        int r = 5;
        for (int i = 0; i < 6; ++i)
            if (grid[i][c] != 0) {
                r = i - 1;
                break;
            }
        return r;
    }
    
    private void quitGame() {
        primaryStage.close();
    }
    
    public VBox getMenu() {
        return menu;
    }
    
    public Pane getController() {
        return controlRoot;
    }
    
}
