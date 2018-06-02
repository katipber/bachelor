package cfs.data.entity;

import java.time.LocalDate;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.OneToOne;

/**
 * Trip entity class
 */

@Entity
public class Trip {
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  private Long      id;
  private String    customer;
  private String    description;
  private LocalDate startdate;
  private LocalDate enddate;
  private String    state;
  private boolean   reservation_request;
  
  @ManyToOne
  private Employee employee;
  @OneToOne
  private Employee manager;
  @OneToOne(mappedBy = "trip")
  private Reservation reservation;
  
  @Override
  public int hashCode() {
    return id.hashCode();
  }
  
  @Override
  public boolean equals(Object obj) {
    if(obj == null) return false;
    if(getClass() == obj.getClass())
      return hashCode() == obj.hashCode();
    return false;
  }

  public Trip() {
  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public String getCustomer() {
    return customer;
  }

  public void setCustomer(String customer) {
    this.customer = customer;
  }

  public String getDescription() {
    return description;
  }

  public void setDescription(String description) {
    this.description = description;
  }

  public LocalDate getStart_date() {
    return startdate;
  }

  public void setStart_date(LocalDate start_date) {
    this.startdate = start_date;
  }

  public LocalDate getEnd_date() {
    return enddate;
  }

  public void setEnd_date(LocalDate end_date) {
    this.enddate = end_date;
  }

  public String getState() {
    return state;
  }

  public void setState(String state) {
    this.state = state;
  }

  public Employee getEmployee() {
    return employee;
  }

  public void setEmployee(Employee employee) {
    this.employee = employee;
  }

  public Employee getManager() {
    return manager;
  }

  public void setManager(Employee manager) {
    this.manager = manager;
  }

  public Reservation getReservation() {
    return reservation;
  }

  public void setReservation(Reservation reservation) {
    this.reservation = reservation;
  }
  
  public String getEmployeeEmail() {
    return employee.getEmail();
  }
  
  public boolean isReservation_request() {
    return reservation_request;
  }

  public void setReservation_request(boolean reservation_request) {
    this.reservation_request = reservation_request;
  }
  
  public String getCarPlate() {
    if(reservation == null) return "";
    return reservation.getCarPlate();
  }


}
