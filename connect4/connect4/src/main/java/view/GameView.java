package view;

import static com.katipber.connect4.MainApp.COL;
import static com.katipber.connect4.MainApp.ROW;
import static com.katipber.connect4.MainApp.PAD;
import static com.katipber.connect4.MainApp.SIZE;
import javafx.animation.TranslateTransition;
import javafx.scene.layout.Pane;

import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.scene.shape.Shape;
import javafx.util.Duration;

public class GameView {
    
    
    private Pane root;
    private Pane tokenRoot;
    private static Color p1, p2;
    
    public GameView() {
        
        root = new Pane();
        tokenRoot = new Pane();
        p1 = Color.FIREBRICK;
        p2 = Color.YELLOW;
        
        Shape grid = makeGrid();
        grid.setTranslateX(3 * PAD);
        root.getChildren().add(tokenRoot);
        root.getChildren().add(grid);
    }
    
    public Pane getGrid() {
        return root;
    }
    
    public Circle makeToken(boolean turn) {
        Circle token = new Circle(SIZE / 2, turn ? p1 : p2);
        
        token.setCenterX(SIZE / 2);
        token.setCenterY(SIZE / 2);
        
        token.setTranslateY(3 * PAD);
        
        return token;
    }

    public void placeToken(Circle token, int row, int col) {
        token.setTranslateX(col * (PAD + SIZE) + 6 * PAD);
        tokenRoot.getChildren().add(token);
        
        TranslateTransition animation = new TranslateTransition(Duration.seconds(0.3), token);
        animation.setToY(row * (PAD + SIZE) + 2 * PAD);
        animation.play();
    }
    
    public void clearGrid() {
        tokenRoot.getChildren().clear();
    }
    
    private Shape makeGrid() {
        Shape grid = new Rectangle((PAD + SIZE) * COL + 5 * PAD, (PAD + SIZE) * ROW + 3 * PAD);
        
        for (int i = 0; i < ROW; ++i)
            for (int j = 0; j < COL; ++j) {
                Circle c = new Circle(SIZE / 2);
                c.setCenterX(SIZE / 2);
                c.setCenterY(SIZE / 2);
                c.setTranslateX(j * (SIZE + PAD) + 3 * PAD);
                c.setTranslateY(i * (SIZE + PAD) + 2 * PAD);
                
                grid = Shape.subtract(grid, c);
            }
        
        return grid;
    }
    
}
