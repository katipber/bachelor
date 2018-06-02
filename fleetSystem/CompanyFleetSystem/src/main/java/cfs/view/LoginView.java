package cfs.view;

import com.vaadin.icons.VaadinIcons;
import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewChangeListener.ViewChangeEvent;
import com.vaadin.server.VaadinService;
import com.vaadin.spring.annotation.SpringView;
import com.vaadin.spring.annotation.UIScope;
import com.vaadin.ui.Button;
import com.vaadin.ui.FormLayout;
import com.vaadin.ui.Notification;
import com.vaadin.ui.PasswordField;
import com.vaadin.ui.TextField;
import com.vaadin.ui.VerticalLayout;
import javax.annotation.PostConstruct;
import org.springframework.beans.factory.annotation.Autowired;
import cfs.controller.ReservationControllerInterface;
import cfs.data.entity.Employee;

/**
 * This class implements the login screen.
 * It contains fields such as user's email and password.
 */

@UIScope
@SpringView(name = LoginView.PATH)
public class LoginView extends VerticalLayout implements View {
  public final static String PATH = "login";  
  private TextField     email;
  private PasswordField pass;
  
  /**
   * Autowired interface which is used to perform business operations
   */
  @Autowired
  private ReservationControllerInterface controller;

  /**
   * Initializes the view. It adds all the necessary
   * buttons and fields to the view.
   */
  @PostConstruct
  protected void init() {
    email = new TextField("Email");
    email.setIcon(VaadinIcons.USER);
    
    pass = new PasswordField("Password");
    pass.setIcon(VaadinIcons.LOCK);
    
    Button button = new Button("Login");
    button.addClickListener(e -> login());

    FormLayout layout = new FormLayout();
    layout.addComponents(email, pass, button);
    
    addComponent(layout);
  }
  
  /**
   * Navigates to the main view upon the successfull login.
   */
  @Override
  public void enter(ViewChangeEvent event) {
    if(VaadinService.getCurrentRequest().getWrappedSession().getAttribute("user") != null)
      event.getNavigator().navigateTo("main");
  }

  /**
   * Checks if the user exists, and sets the user to the session upon success.
   */
  private void login() {
    Employee user = controller.login(email.getValue(), pass.getValue());
    if(user == null)
      Notification.show("Login Failed!", Notification.Type.ERROR_MESSAGE);
    else {
      VaadinService.getCurrentRequest().getWrappedSession().setAttribute("user", user);
      getUI().getNavigator().navigateTo(MainView.PATH);
    }
  }
}
