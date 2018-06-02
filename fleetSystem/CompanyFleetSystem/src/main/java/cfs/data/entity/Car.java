/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cfs.data.entity;

import java.time.LocalDate;
import java.util.List;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.OneToMany;

/**
 * Car entity class
 */

@Entity
public class Car {
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  private Long    id;
  private String  plate;
  private String  model;
  private Integer milage;
  private LocalDate maintenance;
  private Integer production_year;
  private String  state;
  
  @OneToMany
  @JoinColumn(name = "reservation_id")
  private List<Reservation> reservation;
  @OneToMany
  @JoinColumn(name = "maintenace_id")
  private List<Maintenance> maintenace;
  
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

  public Car() {
  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public String getPlate() {
    return plate;
  }

  public void setPlate(String plate) {
    this.plate = plate;
  }

  public String getModel() {
    return model;
  }

  public void setModel(String model) {
    this.model = model;
  }

  public Integer getMilage() {
    return milage;
  }

  public void setMilage(Integer milage) {
    this.milage = milage;
  }

  public LocalDate getMaintenance() {
    return maintenance;
  }

  public void setMaintenance(LocalDate maintenance) {
    this.maintenance = maintenance;
  }

  public Integer getProduction_year() {
    return production_year;
  }

  public void setProduction_year(Integer production_year) {
    this.production_year = production_year;
  }

  public String getState() {
    return state;
  }

  public void setState(String state) {
    this.state = state;
  }

  public List<Reservation> getReservation() {
    return reservation;
  }

  public void setReservation(List<Reservation> reservation) {
    this.reservation = reservation;
  }

  public List<Maintenance> getMaintenace() {
    return maintenace;
  }

  public void setMaintenace(List<Maintenance> maintenace) {
    this.maintenace = maintenace;
  }

}
