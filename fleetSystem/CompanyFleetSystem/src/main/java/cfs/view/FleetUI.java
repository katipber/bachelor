package cfs.view;

import com.vaadin.navigator.View;
import com.vaadin.navigator.ViewDisplay;
import com.vaadin.server.VaadinRequest;
import com.vaadin.server.VaadinService;
import com.vaadin.spring.annotation.SpringUI;
import com.vaadin.spring.annotation.SpringViewDisplay;
import com.vaadin.ui.Component;
import com.vaadin.ui.Panel;
import com.vaadin.ui.UI;

/**
 * The main class implementing the user interface of the application.
 * It is responsible for redirecting the user into different views.
 * It implementes the ViewDisplay which is an interface provided
 * by the Vaadin framework.
 */
@SpringUI
@SpringViewDisplay
public class FleetUI extends UI implements ViewDisplay {
  
  private Panel root;
  
  @Override
  protected void init(VaadinRequest request) {
    root = new Panel();
    setContent(root);
    
    if(VaadinService.getCurrentRequest().getWrappedSession().getAttribute("user") == null)
      this.getNavigator().navigateTo(LoginView.PATH);
    else
      this.getNavigator().navigateTo(MainView.PATH);
  }

  @Override
  public void showView(View view) {
    root.setContent((Component) view);
  }
}
