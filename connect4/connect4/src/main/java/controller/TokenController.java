package controller;

import static com.katipber.connect4.MainApp.PAD;
import static com.katipber.connect4.MainApp.ROW;
import static com.katipber.connect4.MainApp.SIZE;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class TokenController extends Rectangle{
    
    public final int col;
    
    public TokenController (int c) {

        super(SIZE, (SIZE + PAD) * ROW + PAD);

        col = c;

        setFill(Color.TRANSPARENT);
        setOnMouseEntered(e -> setFill(Color.rgb(200, 200, 50, 0.3)));
        setOnMouseExited(e -> setFill(Color.TRANSPARENT));
    }
    
}
