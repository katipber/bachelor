package cfs.data.entity;

import java.util.List;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.JoinTable;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.OneToOne;

/**
 * Employee entity class
 */

@Entity
public class Employee {
  @Id
  @GeneratedValue(strategy = GenerationType.AUTO)
  private Long    id;
  private String  first_name;
  private String  second_name;
  private String  email;
  private String  password;
  
  @ManyToMany
  private List<Role> role;
  @ManyToOne
  private Department department;
  @OneToOne
  private Department managing;
  @OneToMany
  @JoinColumn(name = "trip_id")
  private List<Trip> trip;
  @OneToOne
  private Trip       approved_trip;
  
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

  public Employee() {
  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public String getFirst_name() {
    return first_name;
  }

  public void setFirst_name(String first_name) {
    this.first_name = first_name;
  }

  public String getSecond_name() {
    return second_name;
  }

  public void setSecond_name(String second_name) {
    this.second_name = second_name;
  }

  public String getEmail() {
    return email;
  }

  public void setEmail(String email) {
    this.email = email;
  }

  public String getPassword() {
    return password;
  }

  public void setPassword(String password) {
    this.password = password;
  }

  public List<Role> getRole() {
    return role;
  }

  public void setRole(List<Role> role) {
    this.role = role;
  }

  public Department getDepartment() {
    return department;
  }

  public void setDepartment(Department department) {
    this.department = department;
  }

  public Department getManaging() {
    return managing;
  }

  public void setManaging(Department managing) {
    this.managing = managing;
  }

  public List<Trip> getTrip() {
    return trip;
  }

  public void setTrip(List<Trip> trip) {
    this.trip = trip;
  }

  public Trip getApproved_trip() {
    return approved_trip;
  }

  public void setApproved_trip(Trip approved_trip) {
    this.approved_trip = approved_trip;
  }
}
