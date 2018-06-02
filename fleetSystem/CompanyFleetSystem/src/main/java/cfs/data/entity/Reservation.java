package cfs.data.entity;

import java.time.LocalDate;
import java.util.Date;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.OneToOne;

/**
 * Reservation entity class
 */

@Entity
public class Reservation {
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  private Long    id;
  private LocalDate startdate;
  private LocalDate enddate;
  private Integer start_milage;
  private Integer end_milage;
  private String  state;
  
  @OneToOne
  private Trip trip;
  @ManyToOne
  private Car  car;
  
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

  public Reservation() {
  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
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

  public Integer getStart_milage() {
    return start_milage;
  }

  public void setStart_milage(Integer start_milage) {
    this.start_milage = start_milage;
  }

  public Integer getEnd_milage() {
    return end_milage;
  }

  public void setEnd_milage(Integer end_milage) {
    this.end_milage = end_milage;
  }

  public String getState() {
    return state;
  }

  public void setState(String state) {
    this.state = state;
  }

  public Trip getTrip() {
    return trip;
  }

  public void setTrip(Trip trip) {
    this.trip = trip;
  }

  public Car getCar() {
    return car;
  }

  public void setCar(Car car) {
    this.car = car;
  }
  
  public String getEmployee() {
    return this.getTrip().getEmployeeEmail();
  }
  
  public LocalDate getTripStart_date() {
    return this.getTrip().getStart_date();
  }
  
  public LocalDate getTripEnd_date() {
    return this.getTrip().getEnd_date();
  }
  
  public String getCarPlate() {
    return this.getCar().getPlate();
  }

}
